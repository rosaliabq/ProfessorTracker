
#include <iomanip>
#include <cstdlib>
#include <stdio.h>
#include <fstream>

#include <libconfig.hh>

#include "Runnable.h"

using namespace std;
using namespace libconfig;

class ConfigFile {
public:
	public:
	static ConfigFile* GetInstance();

	void writeFile();
	void readFile();

	~ConfigFile();

	string inline getFrontalCascade()
	{
		return pathCascadeFrontal;
	}

	string inline getProfileCascade()
	{
		return pathCascadeProfile;
	}

	bool inline getshowBackground()
	{
		return showBackground;
	}

	bool inline getbodyMask()
	{
		return bodyMask;
	}

	bool inline getAdaptativeBg()
	{
		return adaptativeBg;
	}

private:
	static ConfigFile* pConfigFile;
	bool showBackground, bodyMask, adaptativeBg;
	Config cfg;
	string fileName, pathCascadeFrontal, pathCascadeProfile;
	ConfigFile();
	double detectScale;
	int detectNeigh;
};