#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <time.h>
#include <cstring>

extern char *days[42];
extern int year, month;

//渲染日历表格,星期和年月的函数
void draw_background();
SDL_Texture *txt_texture;

//渲染字体函数
void draw_font(const char *text,int x, int y);

//监听事件的函数
int event_loop();

//销毁所有初始化的对象与内容释放缓存
void deinit();

//渲染时间的函数
void draw_time();

//初始化窗口的函数
void initgraph();
SDL_Renderer *rdr = NULL;
SDL_Window *win = NULL;

//渲染日期的函数
void draw_day(char *days[42]);
TTF_Font *font;
SDL_Surface *txt_surf;
SDL_Surface *win_surf;

//声音回调函数,配合播放闹铃函数使用
void callback(void *userdata, Uint8 *stream, int len);

//播放闹铃的函数
void play_wav();
Uint8 *audio_buf;
Uint32 audio_len;
Uint32 audio_pos;
SDL_AudioDeviceID device_id;

//次主函数
void main_1();

//输入函数
char *text_input();

void main_1()
{
    initgraph();

//    year1 =  text_input();
//    month1 = text_input();
//
//
    event_loop();
//
//
    deinit();
}

//窗口初始化
void initgraph()
{
    //1.窗口初始化
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("init failed: %s", SDL_GetError());
    }

    //2.创建窗口
    win = SDL_CreateWindow("万年历", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1400, 800, SDL_WINDOW_RESIZABLE);
    if (NULL == win)
    {
        SDL_Log("SDL_CREATE_WINDOW failed: %s", SDL_GetError());
    }

    //2.5创建渲染器
    rdr = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

    //3.设置渲染颜色

    //4.清除屏幕(渲染屏幕背景)


    //1.初始化TTF
    if (TTF_Init() < 0)
    {
        printf("ttf_init_error: %s",TTF_GetError());

    }

    //2.打开字体
    font = TTF_OpenFont("C:/Windows/Fonts/STXINWEI.TTF",60);
    if (!font)
    {
        printf("ttf_font_error: %s",TTF_GetError());

    }

    //

}


//渲染呈现函数
void draw_background()
{
    //1.创建与窗口关联的渲染器

    SDL_SetRenderDrawColor(rdr,255,255,255,255);
    SDL_RenderClear(rdr);
    //5.渲染表格
    SDL_SetRenderDrawColor(rdr,0,0,0,255);
    for(int i = 0; i<=7; i++)
    {
//        SDL_RenderDrawLine(rdr,0,100*i,1400,100*i);
//        SDL_RenderDrawLine(rdr,200*(j+1),0,200*(j+1),600);
        for (int j = 0; j <= 7; j++)
        {
            SDL_Rect rect{i*200,(j+1)*100,200,100};
            SDL_RenderDrawRect(rdr,&rect);
        }
    }
//**渲染文字
    char *week[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    char header[20] ;
    sprintf(header,"%d年    %d月",year,month);

    //渲染年月
    draw_font(header,510,30);


    //渲染星期
    for (int i = 0; i < 7; i++)
    {
        draw_font(week[i],i*200+50,120);
    }


    //4.渲染呈现
//    SDL_RenderPresent(rdr);
}


//事件监听
int event_loop()
{
    SDL_Event event;
    //SDL_Point pt;

    while (1)
    {
        draw_background();
        draw_day(days);
        draw_time();
        SDL_RenderPresent(rdr);
        SDL_Delay(100);
        while(SDL_PollEvent(&event))
        {


            switch (event.type)
            {
                //监听退出事件
                case SDL_QUIT:
                    return 1;

                //监听窗口事件
//                case SDL_WINDOWEVENT:
//                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)//窗口大小改变事件
//                    {
//                        goto name;
//                    }

                //监听鼠标移动事件
                //case SDL_MOUSEMOTION:

                    //鼠标横坐标event.motion.x
                    //鼠标纵坐标event.motion.y
                   // break;
                //监听鼠标按下事件
                //case SDL_MOUSEBUTTONDOWN:

                    //鼠标按下坐标event.button.x   event.button.y
                    //鼠标按下索引event.button.button
                    //鼠标点击次数event.button.clicks
//                    pt = {event.button.x,event.button.y}
//                    if ()
                    //break;
                //监听鼠标按下抬起事件
                //case SDL_MOUSEBUTTONUP:
                    //鼠标抬起坐标event.button.x   event.button.y
                    //鼠标抬起索引event.button.button
                    //鼠标点击次数event.button.clicks
                   // break;
                //监听键盘按下事件
                //case SDL_KEYDOWN:
                    //物理编码event.key.keysym.sym
                    //虚拟编码event.key.keysym.scancode

                    //break;

                //监听键盘抬起事件
                //case SDL_KEYUP:
                    //物理编码event.key.keysym.sym
                    // 虚拟编码event.key.keysym.scancode
                    //break;


                //case SDL_TEXTEDITING:
                    //break;
                //case SDL_TEXTINPUT:





                
            }

        }
    }
}

//销毁窗口
void deinit()
{


    SDL_FreeSurface(win_surf);
    SDL_FreeSurface(txt_surf);
    SDL_DestroyTexture(txt_texture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(rdr);
    SDL_DestroyWindow(win);
    SDL_Quit();
}


//定义播放回调函数
void callback(void *userdata, Uint8 *stream, int len)
{
    int remain = audio_len - audio_pos;
    if (remain > len)
    {
        SDL_memcpy(stream, audio_buf + audio_pos, len);
        audio_pos += len;
    }
    else
    {
        SDL_memcpy(stream, audio_buf + audio_pos, remain);
        audio_pos = 0;
    }
}


//播放声音
void play_wav()
{
    SDL_AudioSpec audio_spec;
    //1.导入WAV文件
    //SDL_LoadWAV('文件名',音频空间地址,音频数据地址,音频字节地址);
    SDL_LoadWAV("ring.wav",&audio_spec,&audio_buf,&audio_len);


    //2.定义播放回调函数
    audio_spec.callback = callback;

    //3.打开音频设备
     device_id = SDL_OpenAudioDevice(NULL,0,&audio_spec,NULL,0);

    //4.开始播放
     SDL_PauseAudioDevice(device_id,0);


    //5.释放资源 关闭设备
    SDL_FreeWAV(audio_buf);
    SDL_CloseAudioDevice(device_id);
}

//显示日期文字
void draw_day(char *days[35])
{
    //3.渲染字体
    for (int i = 0,j = 0,y = 200; i < 42; i++,j++)
    {
        if (j == 7)
        {
            j = 0;
            y += 100;
        }
        draw_font(days[i],j*200+80,y+15);
    }

    //7.释放与销毁资源


}

void draw_time()
{
    time_t current_time = time(NULL);
    struct tm* timeinfo = localtime(&current_time);
    char time_string[9];
    strftime(time_string,9,"%H:%M:%S",timeinfo);
    draw_font(time_string,1100,30);

}

void draw_font(const char *text,int x, int y)
{
    txt_surf = TTF_RenderUTF8_Blended(font,text, {});
    txt_texture = SDL_CreateTextureFromSurface(rdr,txt_surf);
    SDL_Rect txt_rect{x,y,txt_surf->w,txt_surf->h};
    SDL_RenderCopy(rdr,txt_texture,NULL,&txt_rect);
}

//char *text_input()
//{
//    SDL_Event event;
//    char *text;
//    char *editing;
//    while (1)
//    {
//        while (SDL_PollEvent(&event))
//        {
//            if (event.type == SDL_QUIT)
//            {
//                return text;
//            }
//            if (event.type == SDL_TEXTINPUT)
//            {
//                text = strdup(event.text.text);
//                draw_font(editing,0,100);
//                SDL_RenderPresent(rdr);
//
//            }
//            if (event.type == SDL_TEXTEDITING)
//            {
//                editing = strdup(event.edit.text);
//                //draw_font(editing,0,100);
//                //SDL_RenderPresent(rdr);
//
//
//            }
//
//        }
//        //draw_font(text,0,0);
////        draw_font(editing,0,100);
//        //SDL_RenderPresent(rdr);
//    }
//
//}
