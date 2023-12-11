#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "Pointers.h"
#include "jancok.h"
#include "Includes.h"
#include "Decl.h"


SetValue sv{};
int screenWidth = 0;
int screenHeight = 0;
bool g_Initialized = false;
ImGuiWindow* g_window = NULL;
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;
char *buf = NULL;				// Set Buffer
char *rtn = NULL;				// Set Buffer
int len;

// jstringconvertchar*
char *jstringToChar(JNIEnv * env, jstring jstr)
{
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char *)malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}
#include <iostream>
int toUnicode(const char* str)
{
	return str[0] + (str[1] ? toUnicode(str + 1) : 0);
}

struct sConfig {
    struct sESPMenu {
        bool Bones;
        bool Line;
        bool Box;
        bool Health;
        bool Name;
        bool Distance;
        bool TeamID;
        bool Vehicle;
		bool Radar;
        bool zj;
        bool AIMBOT;
        bool RECOIL;
    };
    sESPMenu ESPMenu{0};
	struct sRadar {
        float x;
		float y;
    };
    sRadar Radar{0};
};
sConfig Config{0};


extern "C" {
    JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_init(JNIEnv* env, jclass cls);
    JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_step(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_imgui_Shutdown(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_MotionEventClick(JNIEnv* env, jobject obj,jboolean down,jfloat PosX,jfloat PosY);
	JNIEXPORT jstring JNICALL Java_com_mycompany_application_GLES3JNIView_getWindowRect(JNIEnv *env, jobject thiz);
	JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_real(JNIEnv* env, jobject obj, jint width, jint height);
};

JNIEXPORT void JNICALL
Java_com_mycompany_application_GLES3JNIView_init(JNIEnv* env, jclass cls) {
     // SetUpImGuiContext
    if(g_Initialized) return ;
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Disable load/save. ini file on disk
    // FIXME: Consider using LoadIniSettingsFromMemory() / SaveIniSettingsToMemory() to save in appropriate location for Android.
    io.IniFilename = NULL;
	
	 // Set ImGui Style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();
    
    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init();
    ImGui_ImplOpenGL3_Init("#version 300 es");
	
	jfieldID Id = env->GetStaticFieldID(cls,"fontData", "[B");
	jbyteArray byteArray = (jbyteArray)env->GetStaticObjectField(cls,Id);
	jbyte* fontData = env->GetByteArrayElements(byteArray,NULL);
	int fontDataSize= env->GetArrayLength(byteArray);
	ImFont* font = io.Fonts->AddFontFromMemoryTTF(fontData, fontDataSize, 45.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    IM_ASSERT(font != NULL);
	
	ImGui::GetStyle().ScaleAllSizes(3.0f);
	
	ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0;
    
    g_Initialized=true;
}

JNIEXPORT void JNICALL
Java_com_mycompany_application_GLES3JNIView_resize(JNIEnv* env, jobject obj, jint width, jint height) {
	screenWidth = (int) width;
    screenHeight = (int) height;
	glViewport(0, 0, width, height);
	ImGuiIO &io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.IniFilename = NULL;
	ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
}
static int i1 =0;
ImFont *font;
/************Custom**************/


void InitGlobalPointers() {
     if (IsPackageRunning("com.studiowildcard.wardrumstudios.ark")) {
         ProcessID = GetProcessID("com.studiowildcard.wardrumstudios.ark");
         if (ProcessID <= 0) return;
         
         BaseAddress = GetBaseAddress(ProcessID, "libUE4.so");
         if (!IsValidAddress(BaseAddress)) return;
         GWorld = Read<long>(BaseAddress + 0x762f4b8);
         if (!IsValidAddress(GWorld)) return;
         GEngine = Read<long>(BaseAddress + 0x762c798);
         if (!IsValidAddress(GEngine)) return;
         GName = Read<long>(BaseAddress + 0x74f0480);        
     }
     return;
}

std::string GetObjectName(long ObjectPointer) {
    std::string StrBuffer = "";   
    int32_t NameID = Read<uint32_t>(ObjectPointer + 0x18);
    long FNamePointer = Read<long>(GName + int(NameID / 0x4000) * 0x8);
    if (!IsValidAddress(FNamePointer)) return "";
    long FName = Read<long>(FNamePointer + int(NameID % 0x4000) * 0x8);
    if (!IsValidAddress(FName)) return "";
    ObjectName pBuffer = Read<ObjectName>(FName + 0x10);
    StrBuffer = pBuffer.Data; 
    return StrBuffer;
}

void Parameter() {
     long Level = Read<long>(GWorld + 0x58); if (!IsValidAddress(Level)) return;
     long WorldSettings = Read<long>(Level + 0x300); if (!IsValidAddress(WorldSettings)) return;
     
     Write<float>(WorldSettings + 0x934, Speed);
}

void ESP() {

}

void Inject() {
    InitGlobalPointers();
    ESP();
    Parameter();
}

void BeginDraw()
{
	ImGuiIO &io = ImGui::GetIO();
	//UI form background color
	//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0F, 0.0F, 0.0F, 0.0F));
	//UI form style
	ImGuiStyle& style = ImGui::GetStyle();
	//Form border rounded corners
	style.WindowRounding = 10.0f;

     if (ImGui::Begin("Menu", 0, ImGuiWindowFlags_NoSavedSettings)) 
	{
        ImGui::SetWindowPos(ImVec2(screenWidth - 10, 15));
        ImGui::SetWindowSize(ImVec2(1000, 670));
 
		g_window = ImGui::GetCurrentWindow();
		//set scrolling
		if (ImGui::BeginTabBar("Tab", ImGuiTabBarFlags_FittingPolicyScroll)) {
			//menu title
			if (ImGui::BeginTabItem("Tab1")) {
                
                ImGui::SliderFloat("Speed", &Speed, 1, 6);
                
                //end of this menu page
				ImGui::EndTabItem();           
			}
            ImGui::EndTabBar();

        }
    }
}

//custom graphics drawing






//Finish
void EndDraw()
{
	ImGuiWindow* window =  ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();
	ImGui::End();
	//ImGui::PopStyleColor();
}


JNIEXPORT void JNICALL
Java_com_mycompany_application_GLES3JNIView_step(JNIEnv* env, jobject obj) {
    
	ImGuiIO& io = ImGui::GetIO();
	
	static bool show_demo_window = false;
    static bool show_MainMenu_window = true;

	// Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
  //  ImGui_ImplAndroid_NewFrame(screenWidth,  screenHeight);//？Settings window
    ImGui::NewFrame();
	
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    //Whether to show the demo layout
	if (show_demo_window)
	{
        ImGui::ShowDemoWindow(&show_demo_window);
	}
	
      
    
	/*User defined area starts*/
	//main menu window
	if (show_MainMenu_window)
	{
		BeginDraw();
	}

	Inject();
	//Finish
	EndDraw();
	/*end of user defined area*/
	
    ImGui::Render();
	glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
}

JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_imgui_Shutdown(JNIEnv* env, jobject obj){
    if (!g_Initialized)
        return;
     // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    g_Initialized=false;
}

JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_MotionEventClick(JNIEnv* env, jobject obj,jboolean down,jfloat PosX,jfloat PosY){
	ImGuiIO & io = ImGui::GetIO();
	io.MouseDown[0] = down;
	io.MousePos = ImVec2(PosX,PosY);
}

JNIEXPORT jstring JNICALL Java_com_mycompany_application_GLES3JNIView_getWindowRect(JNIEnv *env, jobject thiz) {
    //get drawing window
    // TODO: accomplish getWindowSizePos()
    char result[256]="0|0|0|0";
    if(g_window){
        sprintf(result,"%d|%d|%d|%d",(int)g_window->Pos.x,(int)g_window->Pos.y,(int)g_window->Size.x,(int)g_window->Size.y);
    }
    return env->NewStringUTF(result);
}

JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_real(JNIEnv* env, jobject obj, jint w, jint h) {
	screenWidth = (int) w;
    screenHeight = (int) h;
}

extern "C" JNIEXPORT void JNICALL Java_com_mycompany_application_MainActivity_Resolution(JNIEnv *env, jclass clazz, jfloat px, jfloat py) {
    //Set the drawing resolution
    sv.px=px;
    sv.py=py;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_mycompany_application_MainActivity_getReady(JNIEnv *env, jclass clazz) {
    int sockCheck=1;
    if (!Create()) {
        perror("Device Interface Creation Failed\n");
    }
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&sockCheck, sizeof(int));
    if (!Bind()) {
        perror("The Running Port Is Occupied By Another Program\n");
    }
    if (!Listen()) {
        perror("The Listening Port Is Occupied\n");
    }
    if (Accept()) {
        //Send Resolution To Binary
        sv.mode=2;
        send((void*)&sv,sizeof(sv));
    }
}
