#include "ConfigFile.h"

ConfigFile* ConfigFile::pConfigFile= 0;

ConfigFile* ConfigFile::GetInstance(){

	if (!pConfigFile) {
		pConfigFile = new ConfigFile();
	}
	return pConfigFile;
}

ConfigFile::ConfigFile(){

	showBackground = false;

	pathCascadeFrontal = "haarcascade_frontalface_alt.xml";
	pathCascadeProfile = "haarcascade_profileface.xml";
	fileName="config.cfg";
	bodyMask=false;
	adaptativeBg=true;

	ifstream file(fileName.c_str());
	if(file.good()){		
		readFile();
	}else{
		writeFile();
	}
}

void ConfigFile::readFile()
{
	try
	{
		cfg.readFile(fileName.c_str());
	}
	catch(const FileIOException &fioex)
	{
		std::cerr << "I/O error while reading file." << std::endl;
	}
	catch(const ParseException &pex)
	{
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << std::endl;
	}

	Setting &root= cfg.getRoot();

	Setting &functions=root["application"];
	functions.lookupValue("showBackground", showBackground);

	functions.lookupValue("pathFrontalCascade", pathCascadeFrontal);
	functions.lookupValue("pathProfileCascade", pathCascadeProfile);

	functions.lookupValue("bodyMask_active", bodyMask);

	functions.lookupValue("adaptativeBackground", adaptativeBg);
}

void ConfigFile::writeFile()
{

	Setting &root=cfg.getRoot();

	Setting &functions = root.add("application", Setting::TypeGroup);
	functions.add("showBackground", Setting::TypeBoolean) = showBackground;

	functions.add("pathFrontalCascade", Setting::TypeString) = pathCascadeFrontal;
	functions.add("pathProfileCascade", Setting::TypeString) = pathCascadeProfile;

	functions.add("bodyMask_active", Setting::TypeBoolean) = bodyMask;

	functions.add("adaptativeBackground", Setting::TypeBoolean) = adaptativeBg;

	try
	{
		cfg.writeFile(fileName.c_str());
		cerr << "New configuration successfully written to: " << fileName
			<< endl;

	}
	catch(const FileIOException &fioex)
	{
		cerr << "I/O error while writing file: " << fileName << endl;

	}
}