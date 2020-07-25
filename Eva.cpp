#include "FFT.h"
#include <Windows.h>
#include "Include/c_api.h"


int main()
{
	/*char* pMsgBuf = nullptr;

	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, ERROR_ARENA_TRASHED, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr
	);

	if (nMsgLen == 0)
	{
		std::cout <<  "Unidentified error code" << std::endl;
	}

	std::cout << ERROR_ARENA_TRASHED << std::endl;
	std::cout << pMsgBuf << std::endl;
	std::cout << nMsgLen << std::endl;
	std::cout << (&pMsgBuf) << std::endl;
	std::cout << sizeof(nMsgLen) << std::endl;

	std::string errorString(pMsgBuf-1, nMsgLen*2);
	LocalFree(pMsgBuf);

	std::cout << errorString << std::endl;
	*/

	std::cout << TF_Version() << std::endl;
	return 0;
}

