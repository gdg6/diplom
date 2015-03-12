#ifndef __REPORT__
#define __REPORT

class  Report
{
private:
	int id;
	int device_id;
	std::string r_type;
	std::string context;
public:

	 int getId() {
	 	return id;
	 }

	 void setId(int id) {
	 	this -> id = id;
	 } 

	 int getDeviceId() {
	 	reutrn device_id;
	 }

	 void setDeviceId(int device_id) {
	 	this -> device_id = device_id;
	 } 

	 std::string getRType() {
	 	reutrn r_type;
	 }

	 void setRType(std::string r_type) {
	 	this -> r_type = r_type;
	 }

	 std::string getContext() {
	 	return context;
	 }

	 void setContext() {
	 	this -> context = context;
	 }
	
};

#endif