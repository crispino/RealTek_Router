/**
 * @file xml_conf.h
 * @brief use tinyxml read xml config
 * @author liuyuan
 * @version 1.0
 * @date 2013-10-16
 */
#ifndef __XML_CONFIG_FILE_20131016_AFJQPJFQ_233__
#define __XML_CONFIG_FILE_20131016_AFJQPJFQ_233__

#include <string>
#include <vector>
#include "tinyxml/tinyxml.h"

using namespace std;

/**
 * @brief usage:
 *	 open: cf.init("../conf/myconf.xml")
 *   read: cf["/root/name"];
 *
 */
class ConfFile{
	public:
		ConfFile(){
			_xml = new TiXmlDocument();
		}
		~ConfFile(){
			delete _xml;
		}
		bool init(const string& pathfile){
			return _xml->LoadFile(pathfile);
		}
		void decompose(string & path, string & nodename){
			nodename.clear();
			if(path.empty() || path[0] != '/'){
				return;
			}
			std::size_t nend = path.find('/',1);
			nodename = path.substr(1, nend - 1);//para:beg,len
			if(nend == string::npos){
				path="";
			}else{
				path = path.substr(nend);
			}
		}

		/**
		 * @brief get multi values of the same name
		 *
		 * @param xpath
		 *
		 * @return 
		 */
		const vector<string> get_values(const string& xpath){
			vector<string> values;
			if(xpath.empty()) {
				return values;
			}
			TiXmlElement* node = _xml->RootElement();
			string nodename, remain_path(xpath);
			decompose(remain_path, nodename);
			while(node){
				if(nodename.empty()){
					return values;
				}
				if(node->ValueStr() == nodename){//value is the elem name
					if(remain_path.empty()){
						//return node->GetText() == NULL ? "" : node->GetText();//<value>text</value>
						if(node->GetText() == NULL){
							return values;
						}else{
							values.push_back(node->GetText());
							node = node->NextSiblingElement();
							continue;
						}
					}
					decompose(remain_path, nodename);
					node = node->FirstChildElement();
					continue;
				}else{
					node = node->NextSiblingElement();
				}
			}
			return values;
		}

		/**
		 * @brief get first value with the name;
		 *
		 * @param xpath
		 *
		 * @return 
		 */
		const string operator[](const string& xpath){
			if(xpath.empty()) {
				return "";
			}
			TiXmlElement* node = _xml->RootElement();
			string nodename, remain_path(xpath);
			decompose(remain_path, nodename);
			while(node){
				if(nodename.empty()){
					return "";
				}
				if(node->ValueStr() == nodename){//value is the elem name
					if(remain_path.empty()){
						return node->GetText() == NULL ? "" : node->GetText();//<value>text</value>
					}
					decompose(remain_path, nodename);
					node = node->FirstChildElement();
					continue;
				}else{
					node = node->NextSiblingElement();
				}
			}
			return "";
		}
	private:
		TiXmlDocument* _xml;
};

#endif
