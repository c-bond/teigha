#include "bproc_main.h"
class BprocWorker {
public:
  BprocWorker(OdDgDatabasePtr& dgndb, OdString& workfile) 
		  : dgn_db(dgndb), work_file(workfile) {};
	~BprocWorker();

  bool do_work();

private:
	bool prep_db();
	bool compress_file();
  bool export_pdf();
	OdDgDatabasePtr dgn_db;
	OdString work_file;
};
