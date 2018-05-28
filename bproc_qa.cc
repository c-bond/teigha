#include "bproc_qa.h"

BprocQa::~BprocQa() {};

// Public entry point
bool BprocQa::do_work() {
	create_rule_list();
  create_qa_map();
	return run_qa();
}

void BprocQa::create_rule_list() {
  Rule rules;

	rules.name = "Single Sheet Check";
	rules.rule_id = 0;
	rules.param1 = 1;
	rules.param2 = 1;
	rules.param3 = 1;
	rule_list.push_back(rules);

	rules.name = "Active Scale";
	rules.rule_id = 1;
	rules.param1 = 1;
  rules.param2 = 1;
	rules.param3 = 1;
	rule_list.push_back(rules);

	rules.name = "Master Unit";
	rules.rule_id = 2;
	rules.param1 = 1;
	rules.param2 = 1;
	rules.param3 = -1;
	rule_list.push_back(rules);
  
	rules.name = "Sub Unit";
	rules.rule_id = 3;
	rules.param1 = 1;
	rules.param2 = 1000;
	rules.param3 = -1;
	rule_list.push_back(rules);

	rules.name = "Global Origin";
	rules.rule_id = 4;
	rules.param1 = 0;
	rules.param2 = 0;
	rules.param3 = 0;
	rule_list.push_back(rules);

	rules.name = "Element Count";
	rules.rule_id = 5;
	rule_list.push_back(rules);

	rules.name = "Ref display";
	rules.rule_id = 6;
	rule_list.push_back(rules);

	rules.name = "Ref nest depth";
	rules.rule_id = 7;
	rule_list.push_back(rules);
}

void BprocQa::create_qa_map() {
	qa_map[0] = &BprocQa::check_sheet_count;	
	qa_map[1] = &BprocQa::check_active_scale;
	qa_map[2] = &BprocQa::check_master_unit;
	qa_map[3] = &BprocQa::check_sub_unit;
	qa_map[4] = &BprocQa::check_global_origin;
  qa_map[5] = &BprocQa::get_ele_count;
	qa_map[6] = &BprocQa::check_ref_display;
	qa_map[7] = &BprocQa::check_ref_nest_depth;
}

bool BprocQa::run_qa() {	
	OdDgModelPtr mod = dgn_db->getActiveModelId().safeOpenObject(OdDg::kForRead);
	loop_refs();
	if(false) {
	for(auto &it : rule_list) {
		results.push_back(it.name);
		std::cout << it.name << ": ";
		(this->*qa_map[it.rule_id])(it) ?
			results.push_back("PASS") : results.push_back("FAIL");
	}
	}
	transform_ele();
	return true;
}

void cnsl(std::string a, std:: string b) {
	std::cout << a << b << '\n';
}

void cnsl(std::string a, int b) {
  std::cout << a << b << '\n';
}

bool BprocQa::check_ref_display(Rule& arg) {
	cnsl("", ref_display);
	return true; 
}

bool BprocQa::check_ref_nest_depth(Rule& arg) {
	cnsl("",ref_nest_depth);
	return true;
}

void BprocQa::loop_refs() {
	ref_display = 0;
	ref_nest_depth = 0;
	OdDgModelPtr mod = dgn_db->getActiveModelId().safeOpenObject(OdDg::kForRead);
	OdDgElementIteratorPtr iter = mod->createControlElementsIterator();
	for( ; !iter->done(); iter->step()) {
	  OdDgElementPtr ele = iter->item().safeOpenObject();
		if(OdDgElement::ElementTypes::kTypeReferenceAttachmentHeader 
		== ele->getElementType()) {
			++ref_count;
			OdDgReferenceAttachmentHeaderPtr att = iter->item().safeOpenObject();
			if(att->getDisplayFlag())
				++ref_display;
			if(att->getNestDepth())
				++ref_nest_depth;
		}
	}
}

struct GridMatrix {
	GridMatrix(double transl_x, double transl_y, double transl_z, double rotation, double scale) 
			: x(transl_x), y(transl_y), z(transl_z), r(rotation), s(scale) {}
	double x, y, z, r, s;
	OdGeMatrix3d get_transform(OdDgModelPtr mod) {
		OdGePoint3d go = mod->getGlobalOrigin();
		OdGeMatrix3d transform_matrix;
		OdGeMatrix3d temp_matrix;
		transform_matrix.setToTranslation(OdGeVector3d(x, y, z));
		transform_matrix *= temp_matrix.setToRotation((r*0.01745329251), OdGeVector3d(0,0,1), go);	
		transform_matrix *= temp_matrix.setToScaling(s, go);
		return transform_matrix;
	}	
};

//GridMatrix LSGCentraltoOS(450607.111, 143235.329, -100, 0.025175967274641, 0.999807558);
GridMatrix LSGCentraltoOS(450607.111, 143235.329, -100, 1.44247667, 0.999807558);
GridMatrix OStoLSGCentral(-454157.420, -131872.044, 100, 358.55752333, 1.000192479);
GridMatrix LSGSouthWesttoOS(450610.266, 143238.209, -100, 1.44189270, 0.999756316);

GridMatrix BitOfBoth(450608.6885,143236.769, -100, 1.44218468, 0.999781937);

bool BprocQa::transform_ele() {
 	OdString temp_path = work_file + "_temp";
	int ele_count = 0, err_count = 0;
	OdDgModelPtr mod = dgn_db->getActiveModelId().safeOpenObject(OdDg::kForRead);
  OdDgElementIteratorPtr iter = mod->createGraphicsElementsIterator();
  OdGeMatrix3d transform_matrix = BitOfBoth.get_transform(mod);
	for( ; !iter->done(); iter->step()) {
		OdDgElementPtr ele = iter->item().safeOpenObject(OdDg::kForWrite);
		OdResult::eOk == ele->transformBy(transform_matrix) ? ++ele_count : ++err_count;
	}
	std::cout << "Transformed " << ele_count << " elements with " << err_count << " errors"  << '\n';
	dgn_db->writeFile(temp_path);
	//if(0 == remove(work_file))
	//  rename(temp_path, work_file);
	return true;
}

bool BprocQa::get_ele_count(Rule& arg) {
 	OdString temp_path = work_file + "_temp";
	int ele_count = 0;
	OdDgModelPtr mod = dgn_db->getActiveModelId().safeOpenObject(OdDg::kForRead);
  OdDgElementIteratorPtr iter = mod->createGraphicsElementsIterator();
  for( ; !iter->done(); iter->step()) {
		OdDgElementPtr ele = iter->item().safeOpenObject(OdDg::kForWrite);
		++ele_count;
	}
	std::cout << "Total graphics element count : " << ele_count << '\n';
	return true;
}

bool BprocQa::check_sheet_count(Rule& arg) {
	int sheet_count = 0;
  OdDgModelTablePtr mod_tab = dgn_db->getModelTable();
	OdDgElementIteratorPtr iter = mod_tab->createIterator();
	for( ; !iter->done(); iter->step()) {
		OdDgModelPtr mod = iter->item().safeOpenObject(OdDg::kForRead);
		std::string modname(mod->getName());
		std::cout << modname << std::endl;

		if(OdDgModel::Type::kSheetModel == mod->getType())
			sheet_count++;
		get_ele_count(arg);	
	}

	std::cout << "Number of sheets : " << sheet_count << std::endl;
	Rule rule;
	rule.name = "hello";
	//check_ref_display(rule);
	
	return true;
}

bool BprocQa::check_active_scale(Rule& arg) {
	double xsc = dgn_db->getXActScale();
	double ysc = dgn_db->getYActScale();
	double zsc = dgn_db->getZActScale();
	std::cout << xsc << ", " << ysc << ", " << zsc << std::endl;
	
	return (arg.param1 == xsc && arg.param2 == ysc && arg.param3 == zsc);
}

bool BprocQa::check_master_unit(Rule& arg) {
	OdDgModelPtr mod = dgn_db->getActiveModelId().safeOpenObject(OdDg::kForRead);
	//if(OdDgModel::UnitMeasure::kMeters == mod->getMasterUnit())
	if(3 == mod->getMasterUnit())
		std::cout << "Meters" << std::endl;
	else
		std::cout << std::endl;
	return true;
}

bool BprocQa::check_sub_unit(Rule& arg) {
	OdDgModelPtr mod = dgn_db->getActiveModelId().safeOpenObject(OdDg::kForRead);
	if(OdDgModel::UnitMeasure::kMillimeters == mod->getSubUnit())
		std::cout << "Millimeters" << std::endl;
	else
		std::cout << std::endl;
	return true;
}

bool BprocQa::check_global_origin(Rule& arg) {
	OdDgModelPtr mod = dgn_db->getActiveModelId().safeOpenObject(OdDg::kForRead);
  OdGePoint3d go = mod->getGlobalOrigin();
	std::cout << std::setprecision(10) << go.x << ", " << go.y << ", " <<
	 	go.z << std::endl;
	
	return (arg.param1 == go.x && arg.param2 == go.y && arg.param3 == go.z);
}
