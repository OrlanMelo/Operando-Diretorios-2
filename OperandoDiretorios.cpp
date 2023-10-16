#include <Windows.h>
#include <fileapi.h>
#include <ShObjIdl_core.h>
#include <string>
#include <iostream>
#include <fstream>
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"shell32.lib")

using namespace std;

class cFuncoes
{
private:

	IFileOperation* Operacao = 0;
	IShellItem* Item;
	IShellItem* Item2;

public:

	void InicializarInstancia()
	{
		CoInitializeEx(0, COINIT_MULTITHREADED);
		CoCreateInstance(CLSID_FileOperation, 0, CLSCTX_ALL, IID_IFileOperation, (void**)&Operacao);
	}

	/*
	* 
	* Ao criar diretórios, você poderá usar duas opções de criação.
	* 
	* 1 - Criar um diretório comum.
	* 2 - Criar um diretório usando os atributos de outro diretório.
	* 
	* Variáveis:
	* - DiretórioAnterior: Irá criar o diretório usando os atributos e propriedades de outro diretório.
	* - NovoDiretorio para criar o diretório usando ou não mesclagem de dados.
	* 
	*/
	void CriarDiretorio(wstring DiretorioAnterior, wstring NovoDiretorio, bool MesclarAtributos)
	{
		BOOL Res;
		if (MesclarAtributos == true)
		{
			Res = CreateDirectoryEx(DiretorioAnterior.c_str(), NovoDiretorio.c_str(), 0);
		}
		else
		{
			Res = CreateDirectory(NovoDiretorio.c_str(), 0);
		}

		switch (Res)
		{
		case TRUE:
			cout << "O diretório foi criado com sucesso..";
			break;
		case ERROR_ALREADY_EXISTS:
			cout << "O diretório informado já existe..\n";
			break;
		case FALSE:
			cout << "Ocorreu um erro durante a operação.." << GetLastError();
			break;
		default:
			break;
		}
	}

	void RenomearDiretorio(wstring Diretorio, wstring NovoNome)
	{
		HRESULT Res;

		SHCreateItemFromParsingName(Diretorio.c_str() /* Nome atual */, 0, IID_PPV_ARGS(&Item));

		Res = Operacao->RenameItem(Item, NovoNome.c_str() /* Novo nome */, 0);
		Operacao->PerformOperations();
	}

	void MoverOuCopiar(bool Mover, wstring Diretorio, wstring NovoLocal)
	{
		SHCreateItemFromParsingName(Diretorio.c_str(), 0, IID_PPV_ARGS(&Item));
		SHCreateItemFromParsingName(NovoLocal.c_str(), 0, IID_PPV_ARGS(&Item2));

		if (Mover == true)
		{
			Operacao->MoveItem(Item /* Local atual */, Item2 /* Novo local */, 0, 0);
		}
		else
		{
			Operacao->CopyItem(Item /* Local atual */, Item2 /* Novo local */, 0, 0);
		}

		Operacao->PerformOperations();

		Item->Release();
		Item2->Release();
	}

	void RemoverDiretorio(wstring Diretorio)
	{
		RemoveDirectory(Diretorio.c_str());
	}

}Funcoes;

int main()
{

	cout << "O assistente está efetuando manipulações e tarefas no diretório...";

	Funcoes.CriarDiretorio(L"C:\\", L"C:\\TESTE", false);

	Funcoes.InicializarInstancia();
	Funcoes.RenomearDiretorio(L"C:\\TESTE", L"TESTE2");
	Funcoes.MoverOuCopiar(false, L"C:\\TESTE2", L"C:\\");
	CoUninitialize();

	Funcoes.RemoverDiretorio(L"C:\\TESTE2");

	system("pause");
}
