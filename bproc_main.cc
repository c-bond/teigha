#include "bproc_main.h"
#include "bproc_worker.h"
#include "bproc_qa.h"

/************************************************************************/
/* Write a message to the console for each ODA_ASSERT                   */
/************************************************************************/
static void customAssertFunc(const char* expr, const char* filename,
	 	int line_number) {
  printf( 
    "Assertion has occured\n"
    " Expression: %s\n"
    " Filename: %s\n"
    " Line number: %d\n",
    expr, filename, line_number );
}

cad_filetype check_filetype(OdString source_path) {
  
	if(0 == source_path.right(4).compare(".dgn"))
		return cad_filetype::dgn;
	else if(0 == source_path.right(4).compare(".dwg"))
		return cad_filetype::dwg;
	else
		return cad_filetype::unknown;
}

#ifndef _TOOLKIT_IN_DLL_
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);
ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO", OdDgn7IOModuleImpl)
ODRX_END_STATIC_MODULE_MAP()
#endif

#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[]) {
#else
int main(int argc, char* argv[]) {
#endif
  
	::odSetAssertFunc(customAssertFunc);
  
  if(argc != 2) {
    oddgPrintConsoleString(L"\nBproc\n");
    oddgPrintConsoleString(L"\tusage: ExDgnDump <source file>\n");
  	return 1;
  }

  OdString source_path = argv[1];
	cad_filetype source_filetype = check_filetype(source_path);
	if(cad_filetype::unknown == source_filetype) {
		std::cout << "Unknown filetype of file: " << source_path.c_str() << std::endl;
		return 1;
	}

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

	OdDbDatabasePtr dwg_db;
  OdDgDatabasePtr dgn_db;
  OdStaticRxObject<MyServices> svcs;
  odrxInitialize(&svcs);
  svcs.disableProgressMeterOutput(true);
	
  try {
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);
    dgn_db = svcs.readFile(source_path);
		
	//	BprocWorker bpw(dgn_db, source_path);
	//	bpw.do_work();
		
		BprocQa dpq(dgn_db, source_path);
		dpq.do_work();

		dgn_db = 0;
	}
  catch (OdError& e) {
    oddgPrintConsoleString(L"\nTeigha\174 for .DGN Error: %ls\n", 
				e.description().c_str());
  }
  catch (...) {
    oddgPrintConsoleString(L"\nUnknown Error.\n");
  }
  ::odrxUninitialize();
}
