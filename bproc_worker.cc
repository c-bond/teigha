#include "bproc_worker.h"
#include <sys/stat.h>
#include <codecvt>
#include <iomanip>

BprocWorker::~BprocWorker() {}

std::string wstring_to_utf8 (const std::wstring& str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.to_bytes(str);
}

long double get_file_size(const wchar_t* fname) {
	struct stat statbuf;
	stat(wstring_to_utf8(fname).c_str(), &statbuf);
	return(long double)statbuf.st_size;
}

OdDg::OdDgCompressDatabaseFlags get_compress_type() {
	unsigned long all = 0x0FFF; // Compress all
	return static_cast<OdDg::OdDgCompressDatabaseFlags>(all); 
}

bool BprocWorker::do_work() {
	return compress_file();
}

bool BprocWorker::compress_file() {
	
	std::cout << "Compressing file - " << wstring_to_utf8(work_file.c_str())
		<< std::endl;
	long double size_before = get_file_size(work_file.c_str());
	dgn_db->compressDatabase(get_compress_type());
	OdString temp_path = work_file + "_temp";
	dgn_db->writeFile(temp_path);
	if(0 == remove(work_file))
	  rename(temp_path, work_file);

	std::cout << "Compression complete" << std::endl;
	long double size_after = get_file_size(work_file.c_str());
  std::cout << "Size before: " << size_before / 1000 << "KB" << std::endl;
  std::cout << "Size after:  "<< size_after / 1000 << "KB" << std::endl;	
	long double pcg_change = (size_before - size_after) / size_before * 100;
	std::cout << "File compressed by: " << std::setprecision(4) << 
		pcg_change << "%" << std::endl;
	
	return true;
}
