structura knopku Fast start

g_Workspace.hFieldMutex = CreateMutex(NULL, TRUE, NULL);
g_Workspace.hRunMutex = CreateMutex(NULL, TRUE, NULL);
if (!SetTimer(IDT_TIMER1, 1000, NULL)){
   ReleaseMutex(g_Workspace.hRunMutex);		
   RedrawWindow();
   _beginthread(this->FastStartProc, 4096, NULL);
   if (g_Workspace.stop == 1){ KillTimer(IDT_TIMER1); }
  }  
CloseHandle(g_Workspace.hFieldMutex);
CloseHandle(g_Workspace.hRunMutex);

structura procedyru FastStart

        do
	{
		WaitForSingleObject(g_Workspace.hFieldMutex, INFINITE);
		\\\\\\\\\\\\\
		vukonuemo algorutm zminu polya
		\\\\\\\\\\\\\
		ReleaseMutex(g_Workspace.hFieldMutex);	
		zminyemo znachennya osnovnogo polya
		g_Workspace.that = g_Workspace.new_t;
	}
	while (WaitForSingleObject(g_Workspace.hRunMutex, 75L) == WAIT_TIMEOUT);
