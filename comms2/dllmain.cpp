// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <stdint.h>
#include <iostream> 
#include "collider.h"

using namespace std;

#define MAX_VERTS 10

struct MyStruct {
	//float xs[MAX_VERTS];
	//float ys[MAX_VERTS];
	 float x0;
	 float x1;
	 float x2;

	 float y0;
	 float y1;
	 float y2;

	  float xp;
	  float yp;
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

	init_col(&col0, A->x0, A->x1, A->x2, A->y0, A->y1, A->y2, A->xp, A->yp);
	init_col(&col1, B->x0, B->x1, B->x2, B->y0, B->y1, B->y2, B->xp, B->yp);

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

