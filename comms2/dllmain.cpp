// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <stdint.h>
#include <iostream> 
#include "collider.h"

using namespace std;

#define MAX_VERTS 10

struct MyStruct {
	float x;
	float y;
	float s;
	float r;
};


void p_root(Node* root) {

	cout << "Printing..." << endl;
	cout << endl;
	Node* temp = root;

	while (temp) {

		cout << temp->dat._x << endl;
		temp = temp->next;
	}
}

#define EXPORTED_METHOD extern "C" __declspec(dllexport)

EXPORTED_METHOD
int CppFunction(MyStruct* A, MyStruct* B)
{
	Collider col0;
	Collider col1;

	init_col(&col0);
	init_col(&col1);

	//col1._pos._x = 3;
	col1._pos._y = A->y;
	col1._pos._x = A->x;

	col0._pos._y = B->y;
	col0._pos._x = B->x;
	//col1._pos._z = 0.5f;

	p_col(col0);
	p_col(col1);

	int flag = intersect(&col0, &col1);

	cout << "hi ... " << flag << endl;

	return flag;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

