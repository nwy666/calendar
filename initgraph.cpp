#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <time.h>
#include <cstring>
#include <SDL2/SDL_image.h>
#include <pthread.h>
#include <unistd.h>

//从main.cpp文件中获取相应月份的日期排布
extern char *days[42];
char *year, *month;
void change_days(int choice);

//渲染日历表格,星期和年月的函数
void draw_background();
SDL_Texture *txt_texture;
SDL_Texture *img_texture;

//渲染字体函数
void draw_font(TTF_Font *font, const char *text, int x, int y, SDL_Color color);
SDL_Color color_1 = {};
SDL_Color color_2 = {200, 0, 0, 255};


//监听窗口事件的函数,主要控制整个主窗口的运行.
int core();

//销毁所有初始化的对象与内容释放缓存
void deinit();

//渲染时间的函数(淘汰使用,性能堪忧)
void draw_time();

//初始化窗口的函数
void initgraph();
SDL_Renderer *rdr = nullptr;
SDL_Window *win = nullptr;

//渲染具体日期的函数
void draw_day(char *days[42]);
TTF_Font *font_1, *font_2, *font_3,*font_4;
SDL_Surface *txt_surf;


//声音回调函数,配合播放闹铃函数使用
void callback(void *userdata, Uint8 *stream, int len);

//播放闹铃的函数
void play_wav();
Uint8 *audio_buf;
Uint32 audio_len;
Uint32 audio_pos;
SDL_AudioDeviceID device_id;

//次主函数,
void main_0();

//输入函数
char *text_input(int x, int y, int choice);//y参数是文字显示的纵坐标,choice参数是年月的选择,1为输入年份,2为输入月份.

//记录日程函数
char *journal_record(int choice,int i);
SDL_Rect rect_1{1080, 120, 150, 70};
SDL_Rect rect_2{1260, 120, 150, 70};
char *time_0 = "";
char *journal = "";
char *title_0 = "";
int times = 0;
char *time_1 = "";
char *title_1 = "";
char *time_2 = "";
char *title_2 = "";

//渲染窗口背景函数
void draw_img();
SDL_Texture *loaded_texture = NULL;

//预处理背景图片函数,优化性能
void preload_img_texture(char *file);
//清理背景图片的texture变量
void cleanup_img_texture();

//渲染具体日志函数
void draw_journal(int i);

//监听窗口函数,主要用于一些弹窗,临时渲染的监听.
void event_loop();

//读取日记文件文本的所有内容(被draw_journal()函数调用)
char *readEntireFile(const char *filename);

//实现文本换行渲染功能(被draw_journal()函数调用)
void renderTextWithNewline(SDL_Renderer *renderer, const char *text, int x, int y);

//删除日记函数,输入要删除的文本起始行与要删除的行数(因为技术不足,所有日记都存储在一个文本中,无法做到删除一个日志,只能选择要删除的行)
void deleteLineFromFile(const char *filename, int lineToDelete, int lines);

//闹钟响铃函数,到了设置时间开始响铃
void *alarmClock(void *arg);
SDL_Rect rect_one{1080, 520, 150, 70};
SDL_Rect rect_tow{1260, 520, 150, 70};
SDL_Rect rect_th{655, 500, 150, 70};
int judge = 0;
char set_time[80];

//设置闹钟函数,设置闹钟时间
void setalarm(int choice,int i);

//删除闹钟函数
void delete_alarm();

//闹钟弹窗函数,到了设置时间,出现响铃的弹窗
void small_window_1();
void small_window_2(int xy);

void del();

//现行函数(将部分函数封装,便于避免bug和修改),通过调用其他功能函数,实现查询输入.
void advance() {
    initgraph();
    preload_img_texture("loong.png");

    year = text_input(600, 0, 1);
    month = text_input(600, 100, 2);

}

//主调函数,调用最主要的core函数
void main_0() {

    core();

    deinit();
}


//窗口初始化
void initgraph() {
    //1.窗口初始化
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        SDL_Log("init failed: %s", SDL_GetError());
    }

    //2.创建窗口
    win = SDL_CreateWindow("万年历", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1440, 800, SDL_WINDOW_RESIZABLE);
    if (nullptr == win) {
        SDL_Log("SDL_CREATE_WINDOW failed: %s", SDL_GetError());
    }
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    //2.5创建渲染器
    rdr = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    //3.设置渲染颜色
    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);

    //4.清除屏幕(渲染屏幕背景)
    SDL_RenderClear(rdr);
    SDL_RenderPresent(rdr);

    //1.初始化字体TTF
    if (TTF_Init() < 0) {
        printf("ttf_init_error: %s", TTF_GetError());

    }

    //2.打开字体
    font_1 = TTF_OpenFont("C:/Windows/Fonts/STXINWEI.TTF", 60);
    font_2 = TTF_OpenFont("C:/Windows/Fonts/simfang.ttf", 30);
    font_3 = TTF_OpenFont("C:/Windows/Fonts/STXINWEI.TTF", 40);
    font_4 = TTF_OpenFont("C:/Windows/Fonts/MISTRAL.TTF", 50);

    //判断字体是否打开成功
    if (!font_1) {
        printf("ttf_font_error: %s", TTF_GetError());

    }
}

//渲染整体主窗口函数
void draw_background() {

    //渲染背景图
    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
    SDL_RenderClear(rdr);
    draw_img();
    //2.渲染表格
    SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
    for (int i = 0; i <= 6; i++) {

        for (int j = 0; j <= 7; j++) {
            SDL_Rect rect{i * 150, (j + 1) * 100, 150, 100};
            SDL_RenderDrawRect(rdr, &rect);
        }
    }
    //3.渲染文字
    char *week[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    char header[20];
    sprintf(header, "< %s年 >   < %s月 >", year, month);

    //3.1渲染年月
    draw_font(font_1, header, 260, 30, color_1);
    SDL_RenderDrawLine(rdr, 1049, 0, 1049, 100);
    SDL_RenderDrawLine(rdr, 1049, 100, 1440, 100);
    SDL_RenderDrawRect(rdr, &rect_1);
    SDL_RenderDrawRect(rdr, &rect_2);

    //3.2渲染星期
    for (int i = 0; i < 7; i++) {
        draw_font(font_1, week[i], i * 150 + 20, 120, color_1);
    }

    //3.3渲染日志与闹钟
    draw_font(font_1, "日     程", 1145, 25, color_1);
    draw_font(font_2, "新建日程    删除日程", 1100, 140, color_1);
    SDL_RenderDrawLine(rdr, 1049, 500, 1440, 500);
    draw_font(font_2, "设置闹钟    删除闹钟", 1100, 540, color_1);
    SDL_RenderDrawRect(rdr, &rect_one);
    SDL_RenderDrawRect(rdr, &rect_tow);
    switch (1) {
        case 1:{
            if (times == 0) break;
            SDL_Rect rect{1049, 199, 391, 102};
            SDL_RenderDrawRect(rdr, &rect);
            draw_font(font_3, time_0, 1097, 210, color_1);
            draw_font(font_2, title_0, 1130, 260, color_1);
            if (times == 1) break;
    }
        case 2:{
            SDL_Rect rect{1049, 299, 391, 102};
            SDL_RenderDrawRect(rdr, &rect);
            draw_font(font_3, time_1, 1097, 310, color_1);
            draw_font(font_2, title_1, 1130, 360, color_1);
            if (times == 2) break;
        }
        case 3:{
            SDL_Rect rect{1049, 399, 391, 102};
            SDL_RenderDrawRect(rdr, &rect);
            draw_font(font_3, time_2, 1097, 410, color_1);
            draw_font(font_2, title_2, 1130, 460, color_1);
            break;
        }
}



    if (judge) {
        SDL_Rect rect{1049, 599, 391, 102};
        SDL_RenderDrawRect(rdr, &rect);
        draw_font(font_3, set_time, 1060, 630, color_1);
    }

}

int dj1 = 0;
int dj2 = 0;
int dj3 = 0;
//事件监听函数的定义
int core() {
    SDL_Event event;
    SDL_Point pt;
    skip:
    draw_background();
    draw_day(days);
//    draw_time();
    SDL_RenderPresent(rdr);
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                //监听退出事件
                case SDL_QUIT:
                    return 1;
                //监听鼠标按下事件
                case SDL_MOUSEBUTTONDOWN: {
                    //鼠标按下坐标event.button.x   event.button.y
                    //鼠标按下索引event.button.button
                    //鼠标点击次数event.button.clicks
                    pt = {event.button.x, event.button.y};
                    SDL_Rect rect{1049, 199, 391, 102};
                    SDL_Rect rect5{1049, 299, 391, 102};
                    SDL_Rect rect6{1049, 399, 391, 102};
                    SDL_Rect rect1{260, 30, 40, 40};
                    SDL_Rect rect2{540, 30, 40, 40};
                    SDL_Rect rect3{630, 30, 40, 40};
                    SDL_Rect rect4{820, 30, 40, 40};
                    SDL_Rect rectmain{0, 200, 1050, 600};
                    if (SDL_PointInRect(&pt, &rect_1)) {
                        if(times == 0) journal_record(1,1);
                        else if(times == 1) journal_record(1,1);
                        else if (times>=2) journal_record(1,1);
                    } else if (SDL_PointInRect(&pt, &rect) && times >= 1) {
                        if (dj1 == 0) draw_journal(1);
                        else if (dj1 == 1) draw_journal(2);
                        else if (dj1 == 2) draw_journal(3);
                    }
                    else if (SDL_PointInRect(&pt, &rect5) && times >= 2) {
                        if (dj2 == 0) draw_journal(2);
                        else if (dj2 == 1) draw_journal(3);
                    }
                    else if (SDL_PointInRect(&pt, &rect6) && times >= 3) {
                        draw_journal(3);
                    } else if (SDL_PointInRect(&pt, &rect_2) && times >= 1) {
                        del();
                        //3.设置渲染颜色
//                        SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
//                        //4.清除屏幕(渲染屏幕背景)
//                        SDL_RenderClear(rdr);
//                        SDL_RenderPresent(rdr);
//                        int x, y;
//                        x = atoi(text_input(650, 0, 5));
//                        y = atoi(text_input(580, 70, 6));
//                        if(times == 1) deleteLineFromFile("journal.txt", x, y);
//                        else if(times == 2) deleteLineFromFile("j1.txt", x, y);
//                        else if (times>=3) deleteLineFromFile("j2.txt", x, y);
                    } else if (SDL_PointInRect(&pt, &rect1)) {
                        change_days(1);
                    }
                    else if (SDL_PointInRect(&pt, &rect2)) {
                        change_days(2);
                    }
                    else if (SDL_PointInRect(&pt, &rect3)) {
                        change_days(3);
                    }
                    else if (SDL_PointInRect(&pt, &rect4)) {
                        change_days(4);
                    }
                    else if (SDL_PointInRect(&pt, &rectmain)) {
                        int xy = (int)event.button.x/150 + (int)((event.button.y-200)/100)*7;
                        small_window_2(xy);
                    }
                    else if (SDL_PointInRect(&pt, &rect_one)) {
                        setalarm(1,1);
                    }
                    else if (SDL_PointInRect(&pt, &rect_tow)) {
                        delete_alarm();
                    }
                    goto skip;
                }
                case SDL_MOUSEBUTTONUP: {
                    pt = {event.button.x, event.button.y};
                    if (SDL_PointInRect(&pt, &rect_th)) {
                        delete_alarm();
                    }
                    goto skip;
                }

            }

        }

    }
}

//销毁缓存
void deinit() {

    deleteLineFromFile("j1.txt", 1, 10);
    deleteLineFromFile("j2.txt", 1, 10);
    deleteLineFromFile("journal.txt", 1, 10);
    cleanup_img_texture();
    SDL_FreeSurface(txt_surf);
    SDL_DestroyTexture(txt_texture);
    SDL_DestroyTexture(img_texture);
    TTF_CloseFont(font_1);
    TTF_CloseFont(font_2);
    TTF_CloseFont(font_3);
    TTF_CloseFont(font_4);
    IMG_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(rdr);
    SDL_DestroyWindow(win);
    SDL_Quit();
}


//定义播放回调函数
void callback(void *userdata, Uint8 *stream, int len) {
    int remain = audio_len - audio_pos;
    if (remain > len) {
        SDL_memcpy(stream, audio_buf + audio_pos, len);
        audio_pos += len;
    } else {
        SDL_memcpy(stream, audio_buf + audio_pos, remain);
        audio_pos = 0;
    }
}


//播放声音函数
void play_wav() {
    SDL_AudioSpec audio_spec;
    //1.导入WAV文件
    //SDL_LoadWAV('文件名',音频空间地址,音频数据地址,音频字节地址);
    SDL_LoadWAV("ring.wav", &audio_spec, &audio_buf, &audio_len);


    //2.定义播放回调函数
    audio_spec.callback = callback;
    small_window_1();


    //3.打开音频设备
    device_id = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);

    //4.开始播放
    SDL_PauseAudioDevice(device_id, 0);
    event_loop();
    //5.释放资源 关闭设备
    SDL_FreeWAV(audio_buf);
    SDL_CloseAudioDevice(device_id);
}

//渲染日期函数的定义
void draw_day(char *days[35]) {
    for (int i = 0, j = 0, y = 200; i < 42; i++, j++) {
        if (j == 7) {
            j = 0;
            y += 100;
        }
        if (i % 7 == 6 || i % 7 == 0) {
            draw_font(font_1, days[i], j * 150 + 50, y + 20, color_2);
        } else {
            draw_font(font_1, days[i], j * 150 + 50, y + 20, color_1);
        }

    }
}

//渲染时间函数的定义
void draw_time() {
    time_t current_time = time(NULL);
    struct tm *timeinfo = localtime(&current_time);
    char time_string[9];
    strftime(time_string, 9, "%H:%M:%S", timeinfo);
    draw_font(font_1, time_string, 750, 30, color_1);

}


//渲染字体函数的定义,形参为字体,文本,渲染位置,渲染颜色(多被调用)
void draw_font(TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    txt_surf = TTF_RenderUTF8_Blended(font, text, {color});
    txt_texture = SDL_CreateTextureFromSurface(rdr, txt_surf);
    SDL_Rect txt_rect{x, y, txt_surf->w, txt_surf->h};
    SDL_RenderCopy(rdr, txt_texture, NULL, &txt_rect);
}


//文本输入函数的定义,形参为横坐标,纵坐标,选择输入类型(多被调用)
char *text_input(int x, int y, int choice) {
    SDL_Event event;
    char *text_0;
    char *text = (char *) malloc(40 * sizeof(char));
    SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
    switch (choice) {
        case 1:
            draw_font(font_1, "请输入要查询的年份:", 0, 0, color_1);
            break;
        case 2:
            draw_font(font_1, "请输入要查询的月份:", 0, 100, color_1);
            break;
        case 3:
            draw_font(font_1, "请输入日期与时间:", 0, 0, color_1);
            break;
        case 4:
            draw_font(font_1, "请输入标题:", 0, 70, color_1);
            break;
        case 5:
            draw_font(font_1, "请输入要删除的起始行:", 0, 0, color_1);
            break;
        case 6:
            draw_font(font_1, "请输入要删除的行数:", 0, 70, color_1);
            break;
        case 7:
            draw_font(font_1, "请输入年份:", 0, 0, color_1);
            break;
        case 8:
            draw_font(font_1, "请输入月份:", 0, 70, color_1);
            break;
        case 9:
            draw_font(font_1, "请输入日:", 0, 140, color_1);
            break;
        case 10:
            draw_font(font_1, "请输入时:", 0, 210, color_1);
            break;
        case 11:
            draw_font(font_1, "请输入分:", 0, 280, color_1);
            break;

    }
    SDL_RenderPresent(rdr);
    strcpy(text, "");
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return text;

                case SDL_TEXTINPUT:

                    text_0 = strdup(event.text.text);
                    strcat(text, text_0);
                    draw_font(font_1, text, x, y, color_1);
                    SDL_RenderPresent(rdr);
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == 1073741912) {
                        return text;
                    }
                    break;
                    //物理编码event.key.keysym.sym
                    //虚拟编码event.key.keysym.scancode:
                    //'enter'物理编码为"1073741912"
            }
        }
    }
}

int record_time = 0;
//日记记录函数的定义,将输入的文本内容写入日记文件
char *journal_record(int choice,int i) {
    //1.渲染背景
    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
    SDL_RenderClear(rdr);
    draw_img();
    SDL_RenderPresent(rdr);
    FILE *file;
    char*result = (char*)malloc(strlen(year) + strlen(month) + 15);;
    //打开文件记录日志
    if (times == 0) {
        file = fopen("journal.txt", "a+");
        dj1 = 0;
    } else if(times == 1)
    {
        file = fopen("j1.txt", "a+");
        dj2 = 0;
    }
    else if(times >= 2)
    {
        file = fopen("j2.txt", "a+");
    }
    if (file == NULL) {
        printf("Failed to open file.\n");
    }
    int x = times;
    if(x == 0)
    {
        if (choice == 1){
            time_0 = text_input(520, 0, 3);
            title_0 = text_input(330, 70, 4);
            fprintf(file, "时间:%s\n",time_0);
            times++;
            record_time++;
            fprintf(file, "标题:%s\n", title_0);
        }
        else if (choice == 2){
            strcpy(result, year);
            strcat(result, "年 ");
            strcat(result, month);
            strcat(result, "月 ");
            strcat(result, days[i]);
            strcat(result, "日");
            strcat(result, "\0");
            title_0 = text_input(330, 76, 4);
            time_0 = result;
            fprintf(file, "时间:%s\n", result);
            times++;
            record_time++;
            fprintf(file, "标题:%s\n", title_0);
        }
    }
    if(x == 1)
    {
        if (choice == 1){
            time_1 = text_input(520, 0, 3);
            title_1 = text_input(330, 70, 4);
            fprintf(file, "时间:%s\n",time_1);
            times++;
            record_time++;
            fprintf(file, "标题:%s\n", title_1);
        }
        else if (choice == 2){
            strcpy(result, year);
            strcat(result, "年 ");
            strcat(result, month);
            strcat(result, "月 ");
            strcat(result, days[i]);
            strcat(result, "日");
            strcat(result, "\0");
            title_1 = text_input(330, 76, 4);
            time_1 = result;
            fprintf(file, "时间:%s\n", result);
            times++;
            record_time++;
            fprintf(file, "标题:%s\n", title_1);
        }
    }
    if(x >= 2)
    {
        if (choice == 1){
            time_2 = text_input(520, 0, 3);
            title_2 = text_input(330, 70, 4);
            fprintf(file, "时间:%s\n",time_2);
            times++;
            record_time++;
            fprintf(file, "标题:%s\n", title_2);
        }
        else if (choice == 2){
            strcpy(result, year);
            strcat(result, "年 ");
            strcat(result, month);
            strcat(result, "月 ");
            strcat(result, days[i]);
            strcat(result, "日");
            strcat(result, "\0");
            title_2 = text_input(330, 76, 4);
            time_2 = result;
            fprintf(file, "时间:%s\n", result);
            times++;
            record_time++;
            fprintf(file, "标题:%s\n", title_2);
        }
    }
    //2.记录时间与标题


    //3.重新渲染背景进行内容输入
    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
    SDL_RenderClear(rdr);
    draw_img();
    SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
    for (int i = 1; i <= 11; i++) {
        SDL_RenderDrawLine(rdr, 0, (i + 1.4) * 70, 1440, (i + 1.4) * 70);
    }

    SDL_RenderPresent(rdr);

    SDL_Event event;
    char *text_0;
    char *text = (char *) malloc(1000);
    char *text_line;
    text_line = (char *) malloc(120);
    int y = 50;
    //5.

    strcpy(text, "");
    strcpy(text_line, "       ");
    draw_font(font_1, "请输入内容:", 0, 0, color_1);
    SDL_RenderPresent(rdr);
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return text;

                case SDL_TEXTINPUT:

                    text_0 = strdup(event.text.text);
                    strcat(text, text_0);
                    strcat(text_line, text_0);
                    draw_font(font_1, text_line, 0, y + 60, color_1);
                    SDL_RenderPresent(rdr);
                    if (strlen(text_line) >= 68) {
                        free(text_line);
                        text_line = (char *) malloc(120);
                        strcpy(text_line, "");
                        y += 70;
                        strcat(text, "\n");
                    }
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == 1073741912) {
                        fprintf(file, "内容:%s\n \n", text);
                        fclose(file);
                        return text;
                    }
                    break;
                    //物理编码event.key.keysym.sym
                    //虚拟编码event.key.keysym.scancode:
                    //'enter'物理编码为"1073741912"

            }
        }
    }
}



void preload_img_texture(char *file) {
// 如果已经加载了纹理，先销毁之前的纹理
    if (loaded_texture != NULL) {
        SDL_DestroyTexture(loaded_texture);
        loaded_texture = NULL;
    }
    SDL_Surface *img_surf = IMG_Load(file);
    if (!img_surf) {
        fprintf(stderr, "Unable to load image %s: %s\n", file, IMG_GetError());
        return;
    }
    loaded_texture = SDL_CreateTextureFromSurface(rdr, img_surf);
    if (!loaded_texture) {
        fprintf(stderr, "Unable to create texture from %s: %s\n", file, SDL_GetError());
    }
    SDL_FreeSurface(img_surf);
}

// 使用预加载的纹理进行渲染
void draw_img() {
    if (!loaded_texture) {
        fprintf(stderr, "Texture not loaded!\n");
        return;
    }
    SDL_Rect txt_rect = {0, 0, 1440, 800};
    SDL_RenderCopy(rdr, loaded_texture, NULL, &txt_rect);
}

// 清理纹理资源
void cleanup_img_texture() {
    if (loaded_texture != NULL) {
        SDL_DestroyTexture(loaded_texture);
        loaded_texture = NULL;
    }
}

//渲染具体日记函数的定义,读取文件文本,将其渲染呈现
void draw_journal(int i) {
    char *text;
    if (i == 1) text = readEntireFile("journal.txt");
    else if (i == 2) text = readEntireFile("j1.txt");
    else if (i>=3) text = readEntireFile("j2.txt");
    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
    SDL_RenderClear(rdr);
    draw_img();
    SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
    for (int i = 1; i <= 11; i++) {
        SDL_RenderDrawLine(rdr, 0, (i) * 64 + 40, 1440, (i) * 64 + 40);
    }
    SDL_RenderPresent(rdr);
    renderTextWithNewline(rdr, text, 10, 50);
    SDL_RenderPresent(rdr);
    event_loop();
}

//事件监听函数的定义,当点击退出,按下回车,点击取消时结束监听
void event_loop() {
    SDL_Event event;
    SDL_Point pt;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                //监听退出事件
                case SDL_QUIT:
                    return;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == 1073741912) return;

                case SDL_MOUSEBUTTONDOWN:
                    pt = {event.button.x, event.button.y};
                    if (SDL_PointInRect(&pt, &rect_th)) {
                        return;
                    }
            }
        }
    }
}


char *readEntireFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 0;
    }

    fseek(file, 0, SEEK_END); // 将文件指针移动到文件末尾
    long fileSize = ftell(file); // 获取文件大小
    fseek(file, 0, SEEK_SET); // 将文件指针移动回文件开头

    char *buffer = (char *) malloc(fileSize + 1); // 分配足够大的内存来存储文件内容
    if (buffer == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return 0;
    }
    fread(buffer, 1, fileSize, file); // 读取文件内容到buffer
    buffer[fileSize] = '\0'; // 添加字符串结束符
    fclose(file); // 关闭文件

    return buffer;// 输出文件内容

    free(buffer); // 释放内存

}

void renderTextWithNewline(SDL_Renderer *renderer, const char *text, int x, int y) {
    char *textCopy = strdup(text);// 复制文本字符串
    char *token = strtok(textCopy, "\n"); // 使用换行符"\n"分割文本

    while (token != NULL) {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font_1, token, (SDL_Color) {0, 0, 0, 0});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect rect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);

        y += surface->h; // 增加y坐标以实现换行

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        token = strtok(NULL, "\n"); // 继续处理下一行文本
    }

    free(textCopy); // 释放复制的文本字符串

}

void deleteLineFromFile(const char *filename, int lineToDelete, int lines) {
    FILE *originalFile = fopen(filename, "r");
    if (originalFile == NULL) {
        printf("Error opening file");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file");
        fclose(originalFile);
        return;
    }

    char buffer[1024];
    int currentLine = 1, x = lineToDelete;

    while (fgets(buffer, sizeof(buffer), originalFile) != NULL) {
        if (currentLine != lineToDelete) {
            fputs(buffer, tempFile);
        } else {
            if (lineToDelete < x + lines - 1) lineToDelete++;
        }
        currentLine++;
    }

    fclose(originalFile);
    fclose(tempFile);

    remove(filename);
    rename("temp.txt", filename);
}


void *alarmClock(void *arg) {
    struct tm *alarmTime = (struct tm *) arg;
    time_t alarm = mktime(alarmTime);
    time_t currentTime;

//    do {
//        time(&currentTime);
//    } while (difftime(alarm, currentTime) > 0);
//******重点优化*****
    time(&currentTime);
    double time = difftime(alarm, currentTime);
    sleep(time);
    play_wav();
    pthread_exit(NULL);
}

//设置闹钟函数的定义,此函数中创建了一个新线程,在主线程进行的同时可以判定时间播放闹钟
//***重点***
pthread_t tid;
void setalarm(int choice,int i) {
    //3.设置渲染颜色
    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);

    //4.清除屏幕(渲染屏幕背景)
    SDL_RenderClear(rdr);
    SDL_RenderPresent(rdr);
    judge = 1;

    int year_1, month_1, day, hours, minutes;

    pthread_attr_t attr;
    if (choice == 1)
    {
        year_1 = atoi(text_input(320, 0, 7));
        month_1 = atoi(text_input(320, 70, 8));
        day = atoi(text_input(300, 140, 9));
        hours = atoi(text_input(300, 210, 10));
        minutes = atoi(text_input(300, 280, 11));
    }
    if (choice == 2 )
    {
        year_1 = atoi(year);
        month_1 = atoi(month);
        day = atoi(days[i]);
        hours = atoi(text_input(300, 210, 10));
        minutes = atoi(text_input(300, 280, 11));

    }

    struct tm alarmTime = {};
    alarmTime.tm_year = year_1 - 1900;
    alarmTime.tm_mon = month_1 - 1;
    alarmTime.tm_mday = day;
    alarmTime.tm_hour = hours;
    alarmTime.tm_min = minutes;
    alarmTime.tm_sec = 0;
    strftime(set_time, 80, "%Y-%m-%d %H:%M:%S", &alarmTime);
    // 初始化线程属性对象
    pthread_attr_init(&attr);
    // 设置线程属性为分离状态
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    // 创建线程并传递线程属性对象
    pthread_create(&tid, &attr, alarmClock, (void *) &alarmTime);
    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);

    //4.清除屏幕(渲染屏幕背景)
    SDL_RenderClear(rdr);
    draw_font(font_1,"设置完成!",590,300,color_1);
    SDL_RenderPresent(rdr);
    //***困难之一***
    sleep(1);

}

void delete_alarm()
{
    judge = 0;
    pthread_cancel(tid);
}

void small_window_1()
{
//以下注释内容为渲染弹窗背景内容,没有好的想法,暂时搁置,弹窗还是用简约风
//    SDL_Surface* surface = IMG_Load("alarm.jpg");
//    if (surface == NULL) {
//        printf("Unable to load image %s! SDL_image Error: %s\n", "imagePath", IMG_GetError());
//    }
//    SDL_Texture* texture = SDL_CreateTextureFromSurface(rdr, surface);
//    if (texture == NULL) {
//        printf("Unable to create texture from %s! SDL Error: %s\n", "imagePath", SDL_GetError());
//    }
//    SDL_FreeSurface(surface);
    SDL_Rect rect0 = { 520, 200, 400, 400 };
//    SDL_Rect rect1 = { 200, 200, 700, 700 };
    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
    SDL_RenderFillRect(rdr,&rect0);
    SDL_SetRenderDrawColor(rdr, 255, 0, 0, 255);
//    SDL_RenderCopy(rdr, texture, &rect1, &rect0);
    SDL_RenderDrawRect(rdr,&rect_th);
    SDL_RenderDrawRect(rdr,&rect0);
    draw_font(font_4,set_time,550,330,color_2);
    draw_font(font_3,"取消",687,514,color_2);
    SDL_RenderPresent(rdr);

}

void small_window_2(int xy) {
    SDL_Rect rect0 = {520, 200, 400, 400};
    SDL_Rect rect1 = {637, 236, 160, 80};
    SDL_Rect rect2 = {637, 466, 160, 80};
    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
    SDL_RenderFillRect(rdr, &rect0);
    SDL_SetRenderDrawColor(rdr, 255, 0, 0, 255);
//    SDL_RenderCopy(rdr, texture, &rect1, &rect0);
    SDL_RenderDrawRect(rdr, &rect2);
    SDL_RenderDrawRect(rdr, &rect1);
    SDL_RenderDrawRect(rdr, &rect0);
    draw_font(font_1, "日志", 650, 240, color_1);
    draw_font(font_1, "or", 690, 350, color_1);
    draw_font(font_1, "闹钟", 650, 480, color_1);
    SDL_RenderPresent(rdr);
    SDL_Event event;
    SDL_Point pt;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                //监听退出事件
                case SDL_QUIT:
                    return;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == 1073741912) return;
                case SDL_MOUSEBUTTONDOWN:
                    pt = {event.button.x, event.button.y};
                    if (SDL_PointInRect(&pt, &rect1)) {
                        if(times == 0) journal_record(2,xy);
                        else if(times == 1) journal_record(2,xy);
                        else if (times>=2) journal_record(2,xy);
                        return;
                    }
                    if (SDL_PointInRect(&pt, &rect2)) {
                        setalarm(2,xy);
                        return;
                    }
            }
        }
    }
}

void del()
{
    SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
    switch (1) {
        case 1:
            SDL_RenderDrawLine(rdr,1400,205,1420,225);
            SDL_RenderDrawLine(rdr,1400,225,1420,205);
            if (times == 1) break;
        case 2:
            SDL_RenderDrawLine(rdr,1400,305,1420,325);
            SDL_RenderDrawLine(rdr,1400,325,1420,305);
            if (times == 2) break;
        case 3:
            SDL_RenderDrawLine(rdr,1400,405,1420,425);
            SDL_RenderDrawLine(rdr,1400,425,1420,405);
    }
    SDL_RenderPresent(rdr);
    SDL_Rect rect1 = {1400,205,20,20};
    SDL_Rect rect2 = {1400,305,20,20};
    SDL_Rect rect3 = {1400,405,20,20};
    SDL_Event event;
    SDL_Point pt;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                //监听退出事件
                case SDL_QUIT:
                    return;

                case SDL_MOUSEBUTTONDOWN:
                    pt = {event.button.x, event.button.y};
                    if (SDL_PointInRect(&pt, &rect3)) {
                        if (times >=3 ) {
                            times = 2;
                            deleteLineFromFile("j2.txt", 1, 5);
                        }
                    } else if (SDL_PointInRect(&pt, &rect2)){
                        if (times == 2) {
                            times--;
                            if (record_time - times == 2) {
                                deleteLineFromFile("j2.txt", 1, 5);
                            }
                            else deleteLineFromFile("j1.txt", 1, 5);
                        }
                        else if (times >= 3) {
                            deleteLineFromFile("j1.txt", 1, 5);
                            times--;
                            dj2 = 1;
                            strcpy(time_1,time_2);
                            strcpy(title_1,title_2);
                        }
                    }else if (SDL_PointInRect(&pt, &rect1)){
                        if (times == 1) {
                            times--;
                            deleteLineFromFile("journal.txt", 1, 5);
                            if (record_time - times == 3) {
                                deleteLineFromFile("j2.txt", 1, 5);
                            }
                        }
                        else if (times == 2)
                        {
                            times--;
                            deleteLineFromFile("journal.txt", 1, 5);
                            dj1 = 1;
                            strcpy(time_0,time_1);
                            strcpy(title_0,title_1);
                            if (record_time - times == 2) {
                                dj1 = 2;
                                deleteLineFromFile("j1.txt", 1, 5);
                            }

                        }
                        else if (times >= 3){
                            times = 2;
                            deleteLineFromFile("journal.txt", 1, 5);
                            dj1 = 1;
                            dj2 = 1;
                            strcpy(time_0,time_1);
                            strcpy(title_0,title_1);
                            strcpy(time_1,time_2);
                            strcpy(title_1,title_2);
                        }

                    }return;
            }
        }

    }

}


