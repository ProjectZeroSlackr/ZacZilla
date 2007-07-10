/*iZilla. Coded by Michael Sweeney(ichaelmay8).*/

/*THINGS TO REMEMBER WHILE TESTING ON THE 5.5G:
         
         Text doesn't seem to work on the 5.5g
         The 5.5g is extremely slow. It's not this way on other iPods(right?)



	NOTE on Windows: Use a ttk function to resize the background image and draw it inside of a window if it opens.
	Make this a default option, let users apply images to windows on their own(IE, In filebrowser, click a file,
	go to ->apply to window.)

	NOTE on Filebrowser: The filebrowser code will have to go within izilla_draw. This way, it can get at the srf
	being drawn to the screen. It can't draw to the screen in it's own function.

    TODO: Ditch the mouse png, exchange it with a mouse drawn from memory.


    IDEA!!: Make a utility to change the background. It'll scan a given directory for drawable images.
            When it finds one, load it into memory along with it's path. Then, scale the image to a small
            enough size, and draw it on the screen, the user can pick from these when drawn. 
            
            Bonus points: draw the images on a 3d flippy thingy

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../pz.h"
#include <errno.h>
#include <ttk.h>
#include <string.h>
#include <dirent.h>


//static int startup();
//static int loadgraphics();
//static int filebrowser();
//static int checkclick(int check_cursorx, int check_cursory);

//extern console_executescript(char *path);
void setupthemes(char *theme);

int usb_connected;



static int imgw, imgh;

static int flagstartup;

static char desktopbackground_path[500];

ttk_surface desktopbackground;
ttk_surface cursor;
ttk_surface menubar;
ttk_surface window;
ttk_surface usbconnected;
ttk_surface infobox;
ttk_surface dock;
ttk_surface dropdownmenu;
static ttk_surface izilla_logo;
static ttk_surface izilla_logo_loader;
static ttk_surface loadbit;
static ttk_surface musicapp;

/*Window stuff*/
ttk_surface wid_button_close;
ttk_surface wid_button_minimize;
static ttk_surface wid_button_scroller;

/*User Instruction stuffiz*/
static ttk_surface pushfw;
static ttk_surface pushrw;
static ttk_surface pushplay;
static ttk_surface pushmenu;
static ttk_surface pushcenter;
static ttk_surface pushclock;
static ttk_surface pushcounter;

ttk_surface miniwindow;
ttk_surface icon_harddrive;

static int cursorx=100;
static int cursory=100;

static int newuser;

static int click=0;

static char startupscriptpath[200];//Baconscript startupscript path

static int popupwindow=1;
static char popupwindowtext[100];
static char popupwindowtitle[10]="<!>";
static char buttonchoice2[10];

static char ipodname[20]="Sven Jr";

static int usboperational=0;//1=working
static int notifyflag=1;

static int filebrowser_open=0;//Don't open it on startup

static int hdiconx=280;
static int hdicony=50;

static float hdfree;
static float hdfull;

static char buffer[200];

//logo flag
static int logoflag=1;

//Menubar Flags
static int flag_izillamenu=0;

//preselected cursor points
static int cursorpoints_flag=0;//off for now
static int current_cursorpoint=0;
static int cursorpoints_x[100]={25,100,200,290,70};
static int cursorpoints_y[100]={15,15,15,60,180};

/*
Pos1= iZilla menu
pos2= settings menu
pos3= power menu
pos4= harddrive
pos5= music icon
*/

/*Lots of theme yays*/

static char theme_menubar[500];
static char theme_wallpaper[500];
static char theme_hdicon[500];
static char theme_dock[500];
static char theme_menubackground[500];
static char theme_window[500];
static char theme_window_close[500];
static char theme_window_minimize[500];
static char theme_window_scroller[500];
static char theme_infobox[500];
FILE *theme;
FILE *output;


static int hdinfoopen;
int refreshthemes=1;

/*Picture Viewer*/
static int picturevieweropen;
static int currentpicture=1;
static ttk_surface pic1;
static ttk_surface pic2;
static ttk_surface pic3;
static ttk_surface pic4;
static ttk_surface pic5;

static ttk_surface drawnpic1;
static ttk_surface drawnpic2;
static ttk_surface drawnpic3;

static int pictureposition_x[10]={85,50,20};
static int pictureposition_y[10]={110,60,25};

static int tempy=0;

static int themechooseropen=1;
static char themetouse[500]="/usr/lib/izilla/themes/devtheme.inx";
static int number_o_themes;
static int foundthemes;
static int readin[500];
static int printx=60;
static int printy=60;
static int page=1;
static int refreshthemelist=1;
static int themenumber;
static int selectedtheme;
static ttk_surface arrow;
static int aboutwindowopen;
static int themerefresh;

static void izilla_draw(TWidget *wid, ttk_surface srf)
{
       if(refreshthemes==1)
       {
                           setupthemes(themetouse);
                           refreshthemes=0;
       }
       
       
       if(usb_connected==1)
       {
                           ttk_blit_image (usbconnected, srf, wid->x + 263, wid->y + 8);
       }
      if(themerefresh==0)
      {
       if(picturevieweropen==0)
       {
      
       
       ttk_blit_image (desktopbackground, srf, wid->x, wid->y);
       ttk_blit_image (menubar, srf, wid->x, wid->y);
       ttk_blit_image (dock, srf, wid->x+60, wid->y+185);
       ttk_blit_image (musicapp, srf, wid->x+65, wid->y+175);
       ttk_blit_image (icon_harddrive, srf, wid->x+280, wid->y+50);  
       
       ttk_blit_image (icon_harddrive, srf, wid->x+hdiconx, wid->y+hdicony);
       ttk_text (srf, ttk_menufont, wid->x + hdiconx-10, wid->y + hdicony+55, ttk_makecol (BLACK), ipodname);//Draw in the HDNAME
       
       /*if(notifyflag==1 && usboperational==0)//notify the user that usb isn't working
       {*/
                
                
                /*ttk_blit_image (window, srf, wid->x + 20, wid->y + 100);
                ttk_blit_image (usbconnected, srf, wid->x + 21, wid->y + 21);
                ttk_text (srf, ttk_menufont, wid->x + 60, wid->y + 100, ttk_makecol (BLACK), "USB"); 
                ttk_text (srf, ttk_menufont, wid->x + 22, wid->y + 120, ttk_makecol (BLACK), "USB is not working");   
                notifyflag=0;*/
       /*}
       else
       {
                
       }*/
      }
       if(filebrowser_open==1)
       {
                              	ttk_blit_image (window, srf, wid->x + 50, wid->y + 26);
                              	ttk_blit_image (wid_button_close, srf, wid->x + 50, wid->y + 26+1);
                              	ttk_blit_image (wid_button_minimize, srf, wid->x + 50+199, wid->y + 26+1);
                                ttk_text (srf, ttk_menufont, wid->x+100, wid->y+30, ttk_makecol (BLACK), "File Browser - N/A");
                                ttk_text (srf, ttk_menufont, wid->x+100, wid->y+75, ttk_makecol (BLACK), "Feature Not Available");  
       }
       if(filebrowser_open==1 && cursorx>50 && cursory>26+1 && cursorx<70 && cursory<46+1 && click==1)
       {
                              filebrowser_open=0;
       }
       
       if(filebrowser_open==1 && cursorx>50+199 && cursory>26+1 && cursorx<50+199+20 && cursory<46+1 && click==1)//user clicked minimize button
       {
                              filebrowser_open=2;//minimize it
                              
       }
       
       if(filebrowser_open==2)
       {
                              ttk_blit_image (miniwindow, srf, wid->x + 100, wid->y + 175);
       }
       
       if(filebrowser_open==2 && cursorx>100 && cursorx<132 && cursory>175 && cursory<197)//User's cursor is over the minimized icon on the dock
       {
                              
                              ttk_text (srf, ttk_menufont, wid->x + cursorx+10, wid->y + cursory-5, ttk_makecol (BLACK), "File browser");
                              
                              if(click==1)
                              {
                                          filebrowser_open=1;//restore the filebrowser
                              }
       }
       
       
       if(flag_izillamenu==1)
       {
                           ttk_blit_image (dropdownmenu, srf, wid->x+0, wid->y+23);  
                           ttk_text (srf, ttk_menufont, wid->x+0, wid->y+25, ttk_makecol (BLACK), "About->");
                           
                           if(cursorx>0 && cursorx<90 && cursory>25 && cursory<40)
                           {
                                        aboutwindowopen=1;
                           }
       }
       if(aboutwindowopen==1)
       {
                            ttk_text (srf, ttk_menufont, wid->x+60, wid->y+26, ttk_makecol (BLACK), "About iZilla 1.0"); 
                            
                            ttk_blit_image (window, srf, wid->x + 50, wid->y + 26);
                           	ttk_blit_image (wid_button_close, srf, wid->x + 50, wid->y + 26+1);
                           	ttk_blit_image (wid_button_minimize, srf, wid->x + 50+199, wid->y + 26+1);
                           	
                           	ttk_text (srf, ttk_menufont, wid->x+100, wid->y+33, ttk_makecol (BLACK), "iZilla 1.0 (2007)");
                           	ttk_text (srf, ttk_menufont, wid->x+60, wid->y+50, ttk_makecol (BLACK), "By Mike Sweeney(ichaelmay8)");
                            ttk_text (srf, ttk_menufont, wid->x+60, wid->y+70, ttk_makecol (BLACK), "help from David Stern(stern)");
                            //ttk_text (srf, ttk_menufont, wid->x+55, wid->y+90, ttk_makecol (BLACK), "http://izilla.sourceforge.net/forums");
                            //ttk_text (srf, ttk_menufont, wid->x+55, wid->y+110, ttk_makecol (BLACK), "http://sourceforge.net/projects/izilla");
                            ttk_text (srf, ttk_menufont, wid->x+60, wid->y+130, ttk_makecol (BLACK), "www.ipodlinux.org/izilla");
       }
       if(aboutwindowopen==1 && cursorx>50 && cursory>26+1 && cursorx<70 && cursory<46+1 && click==1)
       {
                              aboutwindowopen=0;
       }
       
       if(aboutwindowopen==1 && cursorx>50+199 && cursory>26+1 && cursorx<50+199+20 && cursory<46+1 && click==1)//user clicked minimize button
       {
                              aboutwindowopen=2;//minimize it
                              
       }
       
       
       if(aboutwindowopen==2)
       {
                              ttk_blit_image (miniwindow, srf, wid->x + 200, wid->y + 175);
       }
       
       if(aboutwindowopen==2 && cursorx>200 && cursorx<232 && cursory>175 && cursory<197)//User's cursor is over the minimized icon on the dock
       {
                              
                              ttk_text (srf, ttk_menufont, wid->x + cursorx+10, wid->y + cursory-5, ttk_makecol (BLACK), "About iZilla");
                              
                              if(click==1)
                              {
                                          aboutwindowopen=1;//restore the filebrowser
                              }
       }
       //Cursor_Over Stuff
       if(cursorx>hdiconx && cursorx<hdiconx+25)
       {
                                if(cursory>hdicony && cursory<hdicony+25)
                                {
                                                         hdinfoopen=1;
                                                         //The user's cursor is over the HD
                                                         ttk_blit_image (infobox, srf, wid->x + cursorx-121, wid->y + cursory+5);
                                                         if(hdinfoopen==2)
                                                         {
                                                                        ttk_blit_image (pushfw, srf, wid->x + cursorx-29, wid->y + cursory+30);//tell the user that they can push forewards to see more info
                                                         }
                                                         if(hdinfoopen==3)
                                                         {
                                                                          ttk_blit_image (pushrw, srf, wid->x + cursorx-29, wid->y + cursory+30);
                                                         }
                                                         
                                                         strcpy(buffer,"");
                                                         strcpy(buffer,"Free: ");
                                                         strcat(buffer,"N/A");
                                                         
                                                         ttk_text (srf, ttk_menufont, wid->x + cursorx-121, wid->y + cursory+5, ttk_makecol (BLACK), ipodname);
                                                         ttk_text (srf, ttk_menufont, wid->x + cursorx-121, wid->y + cursory+5+20, ttk_makecol (BLACK), buffer);
                                                         if(click==1)
                                                         {
                                                                     filebrowser_open=1;
                                                         }
                                }
       }
       if(cursorx>0 && cursory<20 && cursory>0 && cursorx<60)//user clicked izilla menu
       {
                             flag_izillamenu=1;//izilla menu's down
       }
       
       
       if(cursorx>90 || cursory>101)//if the cursor's not on the iZilla menubutton or the dropdown, close the dropdown menu.
       {
                        flag_izillamenu=0;
       }            
          
       if(logoflag==1)
       {
                      ttk_blit_image (izilla_logo, srf, wid->x + 50, wid->y + 50);//draw the logo
       }
       
       if(picturevieweropen==1)//ignore this crap
       {
                            if(currentpicture==1)
                            {   
                                ttk_blit_image (pic3, srf, wid->x + pictureposition_x[0], wid->y + pictureposition_y[0]);
                                ttk_blit_image (pic2, srf, wid->x + pictureposition_x[1], wid->y + pictureposition_y[1]);
                                ttk_blit_image (pic1, srf, wid->x + pictureposition_x[2], wid->y + pictureposition_y[2]);  
                            }
                            if(currentpicture==2)
                            {
                                ttk_blit_image (pic4, srf, wid->x + pictureposition_x[0], wid->y + pictureposition_y[0]);
                                ttk_blit_image (pic3, srf, wid->x + pictureposition_x[1], wid->y + pictureposition_y[1]);
                                ttk_blit_image (pic2, srf, wid->x + pictureposition_x[2], wid->y + pictureposition_y[2]);  
                            }
                            if(currentpicture==3)
                            {
                               ttk_blit_image (pic5, srf, wid->x + pictureposition_x[0], wid->y + pictureposition_y[0]);
                               ttk_blit_image (pic4, srf, wid->x + pictureposition_x[1], wid->y + pictureposition_y[1]);
                               ttk_blit_image (pic3, srf, wid->x + pictureposition_x[2], wid->y + pictureposition_y[2]);  
                            }
                            if(currentpicture==4)
                            {
                               ttk_blit_image (pic1, srf, wid->x + pictureposition_x[0], wid->y + pictureposition_y[0]);
                               ttk_blit_image (pic5, srf, wid->x + pictureposition_x[1], wid->y + pictureposition_y[1]);
                               ttk_blit_image (pic4, srf, wid->x + pictureposition_x[2], wid->y + pictureposition_y[2]);  
                            }
                            if(currentpicture==5)
                            {
                               ttk_blit_image (pic2, srf, wid->x + pictureposition_x[0], wid->y + pictureposition_y[0]);
                               ttk_blit_image (pic1, srf, wid->x + pictureposition_x[1], wid->y + pictureposition_y[1]);
                               ttk_blit_image (pic5, srf, wid->x + pictureposition_x[2], wid->y + pictureposition_y[2]);  
                            }
                            if(currentpicture==6)
                            {
                                                  currentpicture=1;
                            }
                            if(currentpicture==0)
                            {
                                                  currentpicture=5;
                            }
                            if(tempy!=100)
                            {                      
                                                   ttk_blit_image (pic1, srf, wid->x + 50, wid->y + tempy);
                                                   if(tempy<69)
                                                   {
                                                               tempy=tempy+2;
                                                   }
                                                   if(tempy>70)
                                                   {
                                                               tempy=tempy+1;
                                                   }
                            }
                            //ttk_blit_image (pic1, srf, wid->x + 50, wid->y + 100);
       }
       
       if(themechooseropen==1)
       {
                              	ttk_blit_image (window, srf, wid->x + 50, wid->y + 26);
                              	ttk_blit_image (wid_button_close, srf, wid->x + 50, wid->y + 26+1);
                              	ttk_blit_image (wid_button_minimize, srf, wid->x + 50+199, wid->y + 26+1);
                              	//Scan for .inx files. Print the resulting ones in the window.
                              	
                              /*	DIR           *dir;
                               struct dirent *dir_entry;	
                               dir = opendir("themes/");
                               FILE *temp;

                               
                               if(refreshthemelist==1)
                               {
                                                      temp=fopen("temp.txt", "W");                              
                               
                                                      while(0!= (dir_entry = readdir(dir)))
                                                      {
                                                                if (!strcmp(&dir_entry->d_name[strlen(dir_entry->d_name)-4], ".inx"))
                                                                {                       
                                                                                        number_o_themes++;
                                                                                        fprintf(temp, "%i %s\n", number_o_themes, dir_entry->d_name); 
                                                                }
                                                      }
                                                      fprintf(temp, "end");
                                                      fclose(temp);
                               
                                                      temp=fopen("temp.txt", "R");
                               
                                                      while(foundthemes!=2)
                                                      {
                                                    
                                                                           fscanf(temp, "%s", &readin);
                                                                           foundthemes++;
                                                                           printy=printy+20;
                                                                           ttk_text (srf, ttk_menufont, wid->x+printx, wid->y+printy, ttk_makecol (BLACK), readin);
                                                     }
                                                     fclose(temp);
                                                    refreshthemelist=0;
                               }
                               if(click==1 && cursorx>50+155 && cursorx<50+193 && cursory>26+25 && cursory<26+51)
                               {
                                           temp=fopen("temp.txt", "R");
                                           
                                           while(1)
                                           {
                                           
                                                   fscanf(temp, "%i", &themenumber);
                                           
                                                   if(themenumber==selectedtheme)
                                                   {
                                                                         fscanf(temp, "%s", &themetouse);
                                                                         setupthemes(themetouse);
                                                   }
                                           }
                               }*/
                               
                               ttk_text (srf, ttk_menufont, wid->x+80, wid->y+60, ttk_makecol (BLACK), "OldSkool");
                               ttk_text (srf, ttk_menufont, wid->x+80, wid->y+100, ttk_makecol (BLACK), "Devtheme");
                               if(selectedtheme==1)
                               {
                                          ttk_blit_image (arrow, srf, wid->x + 70, wid->y + 60);
                               }
                               if(selectedtheme==2)
                               {
                                         ttk_blit_image (arrow, srf, wid->x + 70, wid->y + 100);
                               }             
                               
       }
       if(themechooseropen==1 && cursorx>50 && cursory>26+1 && cursorx<70 && cursory<46+1 && click==1)
       {
                              themechooseropen=0;
       }
       
       if(themechooseropen==1 && cursorx>50+199 && cursory>26+1 && cursorx<50+199+20 && cursory<46+1 && click==1)//user clicked minimize button
       {
                              themechooseropen=2;//minimize it
                              
       }
       
       
       if(themechooseropen==2)
       {
                              ttk_blit_image (miniwindow, srf, wid->x + 200, wid->y + 175);
       }
       
       if(themechooseropen==2 && cursorx>100 && cursorx<132 && cursory>175 && cursory<197)//User's cursor is over the minimized icon on the dock
       {
                              
                              ttk_text (srf, ttk_menufont, wid->x + cursorx+10, wid->y + cursory-5, ttk_makecol (BLACK), "Theme Chooser");
                              
                              if(click==1)
                              {
                                          themechooseropen=1;//restore the filebrowser
                              }
       }
       if(themechooseropen==1 && cursorx>80 && cursorx<120 && cursory>60 && cursory<80 && click==1)
       {
                              themerefresh=1;
                              
       }
       if(themechooseropen==1 && cursorx>80 && cursorx<120 && cursory>100 && cursory<120 && click==1)
       {
                              themerefresh=2;
                              
       }
       
       
       if(picturevieweropen==0)
       {   
                    ttk_blit_image (cursor, srf, wid->x + cursorx, wid->y + cursory);//cursor always draws last
       }
     }
     if(themerefresh==1)
     {
                 ttk_text (srf, ttk_menufont, wid->x + cursorx+5, wid->y + cursory+5, ttk_makecol (BLACK), "Loading Theme");
                 setupthemes("/usr/lib/izilla/themes/oldskool.inx");
                 themerefresh=0;
     }
     if(themerefresh==2)
     {
                 ttk_text (srf, ttk_menufont, wid->x + cursorx+5, wid->y + cursory+5, ttk_makecol (BLACK), "Loading Theme");
                 setupthemes("/usr/lib/izilla/themes/devtheme.inx");
                 themerefresh=0;
     }       
       
       click=0;
}

void setupthemes(char *theme)
{
       static char readin[500];//for path length

       theme=fopen(theme, "r");
       output=fopen("/usr/lib/izilla/output.txt", "w");//debugging. take out before release
       fprintf(output,"Opened output.txt\nEntering While\n");
       
       FILE *test;
       
       while(strcmp(readin,"end")!=0)
       {
                                    fprintf(output,"In While...\n");
                                    
                                    fscanf(theme,"%s",readin);
                                    fprintf(output, "scanned current line\n");
                                    
                                    if(strcmp(readin,"menubar:")==0)
                                    {
                                                        fprintf(output, "found menubar:, scanning for path\n");
                                                        fscanf(theme, "%s", &theme_menubar);
                                                        fprintf(output, "menubar: %s\n",theme_menubar);
                                                        fprintf(output, "checking integrity of file\n");
                                                        test=fopen(theme_menubar, "r");
                                                        
                                                        if(test==NULL || test==0)
                                                        {
                                                                      fprintf(output, "%s doesn't exist\n",theme_menubar);
                                                        }
                                                        else
                                                        {
                                                                      fprintf(output, "%s exists, continuing loading...\n",theme_menubar);
                                                        }
                                                        strcpy(readin,"");
                                    }
                                    
                                    if(strcmp(readin,"wallpaper:")==0)
                                    {
                                                        fprintf(output, "found wallpaper:, scanning for path\n");
                                                        fscanf(theme, "%s", &theme_wallpaper);
                                                        fprintf(output, "wallpaper: %s\n", theme_wallpaper);
                                    }
                                    if(strcmp(readin,"hdicon:")==0)
                                    {
                                                        fprintf(output, "found hdicon:, scanning for path\n");
                                                        fscanf(theme, "%s", &theme_hdicon);
                                                        fprintf(output, "hdicon: %s\n", theme_hdicon);
                                    }
                                    if(strcmp(readin,"dock:")==0)
                                    {
                                                        fprintf(output, "found dock:, scanning for path\n");
                                                        fscanf(theme, "%s", &theme_dock);
                                                        fprintf(output, "dock: %s\n", theme_dock);
                                    }
                                    if(strcmp(readin,"menubackground:")==0)
                                    {
                                                        fprintf(output, "found menubackground:, scanning for path\n");
                                                        fscanf(theme, "%s", &theme_menubackground);
                                                        fprintf(output, "menubackground: %s\n", theme_menubackground);
                                    }
                                    if(strcmp(readin,"window:")==0)
                                    {
                                                         fprintf(output, "found window:, scanning for path\n");
                                                        fscanf(theme, "%s", &theme_window);
                                                        fprintf(output, "window: %s\n", theme_window);
                                    }
                                    if(strcmp(readin,"window_close:")==0)
                                    {
                                                        fprintf(output, "found window_close:, scanning for path\n");                 
                                                        fscanf(theme, "%s", &theme_window_close);
                                                        fprintf(output, "window_close: %s\n", theme_window_close);
                                    }
                                    if(strcmp(readin,"window_minimize:")==0)
                                    {
                                                        fprintf(output, "found window_minimize:, scanning for path\n");
                                                        fscanf(theme, "%s", &theme_window_minimize);
                                                        fprintf(output, "window_minimize: %s\n", theme_window_minimize);
                                    }
                                    if(strcmp(readin,"window_scroller:")==0)
                                    {
                                                        fprintf(output, "found window_scroller:, scanning for path\n");
                                                        fscanf(theme, "%s", &theme_window_scroller);
                                                        fprintf(output, "window_scroller: %s\n", theme_window_scroller);
                                    }
                                    if(strcmp(readin,"mouseover_infobox:")==0)
                                    {
                                                        fprintf(output, "found mouseover_infobox:, scanning for path\n");
                                                        fscanf(theme, "%s", &theme_infobox);
                                                        fprintf(output, "mouseover_infobox: %s\n", theme_infobox);
                                    }
                                    
       }
       
          /*Base UI Graphics*/
    
    desktopbackground = ttk_load_image ("/mnt/izilla/theme_wallpaper");
    ttk_surface_get_dimen (desktopbackground, &imgw, &imgh);
       
    cursor = ttk_load_image ("/mnt/izilla/themes/devtheme/cursor.png");
    ttk_surface_get_dimen (cursor, &imgw, &imgh);
    
    menubar = ttk_load_image ("/mnt/izilla/theme_menubar");
    ttk_surface_get_dimen (cursor, &imgw, &imgh);

    dock = ttk_load_image ("/mnt/izilla/theme_dock");
    ttk_surface_get_dimen (dock, &imgw, &imgh);

    /*Icons*/    

    icon_harddrive = ttk_load_image ("/mnt/izilla/theme_hdicon");
    ttk_surface_get_dimen (icon_harddrive, &imgw, &imgh);
    
    musicapp = ttk_load_image ("/mnt/izilla/themes/devtheme/music.png");
    ttk_surface_get_dimen (musicapp, &imgw, &imgh);

    
    /*Boxes*/

    infobox = ttk_load_image ("/mnt/izilla/theme_infobox");
    ttk_surface_get_dimen (infobox, &imgw, &imgh);
    
    dropdownmenu = ttk_load_image ("/mnt/izilla/theme_menubackground");
    ttk_surface_get_dimen (dropdownmenu, &imgw, &imgh);
    
    

    /*Status Stuff*/
    
    loadbit = ttk_load_image ("/mnt/izilla/themes/devtheme/loadbit.png");
    ttk_surface_get_dimen (loadbit, &imgw, &imgh);

    usbconnected = ttk_load_image ("/mnt/izilla/themes/devtheme/usbconnected.png");
    ttk_surface_get_dimen (usbconnected, &imgw, &imgh);
    

    
    /*Logos, ect.*/

    izilla_logo = ttk_load_image ("/mnt/izilla/themes/devtheme/logo.png");
    ttk_surface_get_dimen (izilla_logo, &imgw, &imgh);
    
    izilla_logo_loader = ttk_load_image ("/mnt/izilla/themes/devtheme/logoloader.png");
    ttk_surface_get_dimen (izilla_logo_loader, &imgw, &imgh);
    

    
   /*Window Graphics*/

    window = ttk_load_image ("/mnt/izilla/theme_window");
    ttk_surface_get_dimen (window, &imgw, &imgh);

    wid_button_close = ttk_load_image ("/mnt/izilla/theme_window_close");
    ttk_surface_get_dimen (wid_button_close, &imgw, &imgh);

    wid_button_minimize = ttk_load_image ("/mnt/izilla/theme_window_minimize");
    ttk_surface_get_dimen (wid_button_minimize, &imgw, &imgh);

    wid_button_scroller = ttk_load_image ("/mnt/izilla/theme_window_scroller");
    ttk_surface_get_dimen (wid_button_scroller, &imgw, &imgh);
    
    miniwindow = ttk_load_image ("/mnt/izilla/themes/devtheme/miniwindow.png");
    ttk_surface_get_dimen (miniwindow, &imgw, &imgh);
    
    /*User instruction stuffiz*/
    
    pushfw = ttk_load_image ("/mnt/izilla/themes/devtheme/pushfw.png");
    ttk_surface_get_dimen (pushfw, &imgw, &imgh);
    
    pushrw = ttk_load_image ("/mnt/izilla/themes/devtheme/pushrw.png");
    ttk_surface_get_dimen (pushrw, &imgw, &imgh);
    
    pushplay = ttk_load_image ("/mnt/izilla/themes/devtheme/pushplay.png");
    ttk_surface_get_dimen (pushplay, &imgw, &imgh);
    
    pushmenu = ttk_load_image ("/mnt/izilla/themes/devtheme/pushmenu.png");
    ttk_surface_get_dimen (pushmenu, &imgw, &imgh);
    
    pushcenter = ttk_load_image ("/mnt/izilla/themes/devtheme/pushcenter.png");
    ttk_surface_get_dimen (pushfw, &imgw, &imgh);
    
    pushclock = ttk_load_image ("/mnt/izilla/themes/devtheme/pushclock.png");
    ttk_surface_get_dimen (pushclock, &imgw, &imgh);
    
    pushcounter = ttk_load_image ("/mnt/izilla/themes/devtheme/pushcounter.png");
    ttk_surface_get_dimen (pushcounter, &imgw, &imgh);
    
    /*Pictureviewer stuffiz*/
    
    pic1 = ttk_load_image ("/mnt/izilla/pic1.png");
    ttk_surface_get_dimen (pic1, &imgw, &imgh);
    
    pic2 = ttk_load_image ("/mnt/izilla/pic2.png");
    ttk_surface_get_dimen (pic2, &imgw, &imgh);
    
    pic3 = ttk_load_image ("/mnt/izilla/pic3.png");
    ttk_surface_get_dimen (pic3, &imgw, &imgh);
    
    pic4 = ttk_load_image ("/mnt/izilla/pic4.png");
    ttk_surface_get_dimen (pic4, &imgw, &imgh);
    
    pic5 = ttk_load_image ("/mnt/izilla/pic5.png");
    ttk_surface_get_dimen (pic5, &imgw, &imgh);
    
    arrow = ttk_load_image ("/mnt/izilla/arrow.png");
    ttk_surface_get_dimen (arrow, &imgw, &imgh);
       
       refreshthemes=0;
       fclose(output);
}



/*static int startup(void)
{
      /* //usb_check_connection();
       
       //if(usb_check_connection==0)//not connected
       {
                                        //do nothing
       }
       if(usb_check_connection==1)//connected
       {
                                             usboperational=1;
       }*/


//}

/*static int checkclick(int check_cursorx, int check_cursory)
{
       static int result;
       
       if(check_cursorx>hdiconx && check_cursorx<hdiconx+25)
       {
                                if(check_cursory>hdicony && check_cursory<hdicony+25)
                                {
                                                         //The user clicked the HDICON, thus opening the filebrowser
                                                         
                                                         filebrowser_open=1;
                                                         result=1;//we clicked on something
                                }
       }
       if(check_cursorx>0 && check_cursory<20 && check_cursory>0 && check_cursorx<42)//user clicked izilla menu
       {
                             flag_izillamenu=1;//izilla menu's down
                             result=1;
       }
       
       
       return result;
}*/
          
/*static int filebrowser(void)
{
      static char filename[33];
      static char directory[500];
      static int refreshdirectory=1;
      
      DIR           *dir;
      struct dirent *dir_entry;	
      dir = opendir(directory);
      
      //ttk_blit_image (window, srf, wid->x + 20, wid->y + 20);
      
      
      if(refreshdirectory==1)//We only want this to happen once.
      {
                     //while(0!= (dir_entry = readdir(dir))) //what in the world is in that room, whaddya got in that room, a coupla' gatts, a coupla' knives, a coupla' rats, a coupla' wives
                     //{
                          
                          
                          
                          //if (!strcmp(&dir_entry->d_name[strlen(dir_entry->d_name)-4], ".txt"))
                          //{ 
                             
                          
                             strcat(file, dir_entry->d_name);
                             //ttk_text (srf, ttk_menufont, wid->x + 22, wid->y + 21, ttk_makecol (BLACK), dir_entry->d_name);
                          //}
                     //}
                     refreshdirectory=0;
      }

}      */ 
       

static int izilla_scroll(TWidget *this,int dir)
{
	if(cursorpoints_flag==1)
	{
                            cursorx=cursorpoints_x[current_cursorpoint];//move the mouse
                            cursory=cursorpoints_y[current_cursorpoint];//move the mouse
    }
    this->dirty = 1;
    int ret = TTK_EV_CLICK;
           if(picturevieweropen==1)
           {
                           TTK_SCROLLMOD( dir, 4);        
           }
           else
           {
                           TTK_SCROLLMOD( dir, 1);
           }
           if( dir > 0)//counterclockwise?
           {
               if(picturevieweropen==1)
               {
                                       currentpicture++;
               }
               else
               {
                if(cursorpoints_flag==1)//cursorpoints are on
                {
                                 current_cursorpoint++;
                                 /*if(cursorpoints_x[current_cursorpoint]>5)
                                 {
                                                                          current_cursorpoint=0;
                                 }*/
                }
                else                     
                {
                 
                                     cursorx=cursorx+5;
                }
               }
               logoflag=0;
               this->dirty = 1;
        
          }
      
          else//clockwise? 
          {
               if(picturevieweropen==1)
               {
                                       currentpicture--;
               }
               else
               {
                if(cursorpoints_flag==1)//cursorpoints are on
                {
                                 current_cursorpoint--;
                                 /*if(cursorpoints_x[current_cursorpoint]<0)
                                 {
                                                                          current_cursorpoint=5;
                                 }*/
                }
                else                     
                {
                                        cursorx=cursorx-5;
                }
               }
                logoflag=0;
                this->dirty = 1; 
                
          }
          
                         
    return ret;
}



static int izilla_down(TWidget *this,int button)
{
		switch (button) 
        {
		       case TTK_BUTTON_ACTION:
			        if(themechooseropen==1)
			        {
                                           if(selectedtheme==1)
                                           {
                                                               setupthemes("/usr/lib/izilla/themes/devtheme.inx");
                                           }
                                           if(selectedtheme==2)
                                           {
                                                               setupthemes("/usr/lib/izilla/themes/oldskool.inx");
                                           }
                    }
                                           
                    click=1;
                    logoflag=0;
                    popupwindow=0;
			        
                    this->dirty = 1;
                    break;

		       case TTK_BUTTON_MENU:
                    if(picturevieweropen==1)
                    {
                                            picturevieweropen=0;
                    }
                    else
                    {
                        cursory=cursory-5;
                        logoflag=0;
                    }
                    break;
        
               case TTK_BUTTON_NEXT:
                    if(hdinfoopen>1 || hdinfoopen==1)
                    {
                                    hdinfoopen=3;
                    }
                    if(themechooseropen==1)
                    {
                                           switch(selectedtheme)
                                           {
                                                                case 1:
                                                                     selectedtheme=2;
                                                                     break;
                                                                case 2:
                                                                     selectedtheme=1;
                                                                     break;
                                           }
                    }
                    logoflag=0;
                    break;
		  
               case TTK_BUTTON_PREVIOUS:
                    /*if(hdinfoopen>1 || hdinfoopen==1)
                    {
                                     hdinfoopen=2;
                    }
                    else
                    {*/
                        ttk_free_window(this->win);
                    //}
                    break;
               case TTK_BUTTON_PLAY:
                    if(picturevieweropen==0)
                    {
                        cursory=cursory+5;
                    }
                    
                    logoflag=0;
                    
                    break;
    	
		}

	return 0;
}

TWindow *new_izilla_window()
{
	TWindow *ret;
TWidget *wid;

	ret = ttk_new_window("iZilla");
	wid=ttk_new_widget(0,0);
	wid->focusable=1;
	wid->draw=izilla_draw;
	wid->scroll=izilla_scroll;
	wid->down=izilla_down;
	ttk_add_widget(ret, wid);

	ttk_show_window(ret);
}

/*int main()
{
  ttk_init();
  
  new_izilla_window();
  ttk_run();
}*/
