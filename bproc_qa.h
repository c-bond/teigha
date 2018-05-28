#include "bproc_main.h"
#include <map>
#include <functional>
#include <vector>

struct Rule {
	int rule_id;
	std::string name;
	int param1;
	int param2;
	int param3;
};

class BprocQa {
public:

	BprocQa(OdDgDatabasePtr& dgndb, OdString& workfile)
			: dgn_db(dgndb), work_file(workfile) {};
	BprocQa(OdDbDatabasePtr& dwgdb, OdString& workfile)
			: dwg_db(dwgdb), work_file(workfile) {};
	~BprocQa();

	bool do_work();
	std::string get_last_error() const {return last_error;};

private:
  
  typedef bool (BprocQa::*pfunc)(Rule&);

	bool run_qa();
  void create_qa_map();
	void create_rule_list();
	void loop_refs();

	bool check_sheet_count(Rule& arg);
	bool check_active_scale(Rule& arg);
	bool check_master_unit(Rule& arg);
	bool check_sub_unit(Rule& arg);
  bool check_global_origin(Rule& arg);

  bool check_ref_display(Rule& arg);
  bool check_ref_nest_depth(Rule& arg);

  bool get_ele_count(Rule& arg);
  bool transform_ele();
  void set_last_error(std::string& err_msg);
	int ref_count;	
	int ref_display;
	int ref_nest_depth;
	std::vector<std::string> results;  
  std::vector<Rule> rule_list;
	std::string last_error;
	std::map<int, pfunc> qa_map;
	OdDgDatabasePtr dgn_db;
	OdDbDatabasePtr dwg_db;
	OdString work_file;
};
