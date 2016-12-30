#include <vector>
#include "time.h"
#include "algorithm"
#include "output.h"
using namespace std;

int main()
{
	int a[5] = {1,2,3,4,5};

	int * ptr = &a+1;
	printf("%d,%d", *(a+1), *(ptr-1));
	//for (std::string::iterator it=source.begin(); it!=source.end();)
	//{
	//	char tmp = *it; 
	//	it = source.erase(it);

	//	if (' ' == tmp)
	//	{
	//		if (!cmd.empty())
	//			break;
	//		else
	//			continue;
	//	}

	//	cmd+=tmp;
	//}
	//
	//printf("%s\n", cmd.c_str());
	/*std::string param="null";
	
	if ("add" == cmd)
	{
		for (uint32 i=0, size=ntf.Command.length(); i<size; ++i)
		{
			char tmp = ntf.Command[i]; 
			ntf.Command.erase(i);

			if (' ' == tmp)
				break;
			else if (!i)
				cmd.clear();

			param+=tmp;
		}		
	}
	else if ("sortrank" == cmd)
	{
		for (uint32 i=0, size=ntf.Command.length(); i<size; ++i)
		{
			char tmp = ntf.Command[i]; 
			ntf.Command.erase(i);

			if (' ' == tmp)
				break;
			else if (!i)
				cmd.clear();

			param+=tmp;
		}
	}
	else
	{
		sLog.outError("FR-Acty:can not recognize command[%s].", cmd.c_str());
		return;
	}
		
	sLog.outDebug("FR-Acty:ServerID[%d], Command[%s],Parmas[%s]", serverid, cmd.c_str(), param.c_str());*/

	system("pause");
}