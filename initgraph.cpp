#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <time.h>
#include <cstring>
#include <SDL2/SDL_image.h>
#include <pthread.h>
#include <unistd.h>

extern char *days[42];
char *year, *month;

//渲染日历表格,星期和年月的函数
void draw_background();

SDL_Texture *txt_texture;
SDL_Texture *img_texture;

//渲染字体函数
void draw_font(TTF_Font *font, const char *text, int x, int y, SDL_Color color);

SDL_Color color_1 = {};
SDL_Color color_2 = {200, 0, 0, 255};


//监听事件的函数
int core();

//销毁所有初始化的对象与内容释放缓存
void deinit();

//渲染时间的函数
void draw_time();

//初始化窗口的函数
void initgraph();

SDL_Renderer *rdr = nullptr;
SDL_Window *win = nullptr;

//渲染日期的函数
void draw_day(char *days[42]);

TTF_Font *font_1, *font_2, *font_3;
SDL_Surface *txt_surf;


//声音回调函数,配合播放闹铃函数使用
void callback(void *userdata, Uint8 *stream, int len);

//播放闹铃的函数
void play_wav();

Uint8 *audio_buf;
Uint32 audio_len;
Uint32 audio_pos;
SDL_AudioDeviceID device_id;

//次主函数
void main_0();

//输入年月函数
char *text_input(int x, int y, int choice);//y参数是文字显示的纵坐标,choice参数是年月的选择,1为年份,2为月份.

//输入日程函数
char *journal_record();

SDL_Rect rect_1{1080, 120, 150, 70};
SDL_Rect rect_2{1260, 120, 150, 70};
char *time_0 = "";
char *journal = "";
char *title = "";

void draw_img();

SDL_Texture *loaded_texture = NULL;

void preload_img_texture(char *file);

void cleanup_img_texture();

void draw_journal();

void event_loop();

char *readEntireFile(const char *filename);

void renderTextWithNewline(SDL_Renderer *renderer, const char *text, int x, int y);

void deleteLineFromFile(const char *filename, int lineToDelete, int lines);


void *alarmClock(void *arg);

SDL_Rect rect_one{1080, 520, 150, 70};
SDL_Rect rect_tow{1260, 520, 150, 70};

void setalarm();

void advance() {
    initgraph();
    preload_img_texture("loong.png");

    year = text_input(600, 0, 1);
    month = text_input(600, 100, 2);

}

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

    //1.初始化TTF
    if (TTF_Init() < 0) {
        printf("ttf_init_error: %s", TTF_GetError());

    }

    //2.打开字体
    font_1 = TTF_OpenFont("C:/Windows/Fonts/STXINWEI.TTF", 60);
    font_2 = TTF_OpenFont("C:/Windows/Fonts/simfang.ttf", 30);
    font_3 = TTF_OpenFont("C:/Windows/Fonts/STXINWEI.TTF", 40);
    if (!font_1) {
        printf("ttf_font_error: %s", TTF_GetError());

    }

    //

}


//渲染呈现函数
void draw_background() {
    //1.创建与窗口关联的渲染器

    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
    SDL_RenderClear(rdr);
    draw_img();
    //5.渲染表格
    SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
    for (int i = 0; i <= 6; i++) {

        for (int j = 0; j <= 7; j++) {
            SDL_Rect rect{i * 150, (j + 1) * 100, 150, 100};
            SDL_RenderDrawRect(rdr, &rect);
        }
    }
//**渲染文字
    char *week[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    char header[20];
    sprintf(header, "%s年    %s月", year, month);

    //渲染年月
    draw_font(font_1, header, 100, 30, color_1);
    SDL_RenderDrawLine(rdr, 1049, 0, 1049, 100);
    SDL_RenderDrawLine(rdr, 1049, 100, 1440, 100);

    SDL_RenderDrawRect(rdr, &rect_1);

    SDL_RenderDrawRect(rdr, &rect_2);





    //渲染星期
    for (int i = 0; i < 7; i++) {
        draw_font(font_1, week[i], i * 150 + 20, 120, color_1);
    }

    //渲染日志
    draw_font(font_1, "日     程", 1145, 25, color_1);
    draw_font(font_2, "新建日程    删除日程", 1100, 140, color_1);
    SDL_RenderDrawLine(rdr, 1049, 500, 1440, 500);
    draw_font(font_2, "设置闹钟    删除闹钟", 1100, 540, color_1);
    SDL_RenderDrawRect(rdr, &rect_one);
    SDL_RenderDrawRect(rdr, &rect_tow);
    if (strlen(journal) >= 4) {
        SDL_Rect rect{1049, 199, 391, 102};
        SDL_RenderDrawRect(rdr, &rect);
        draw_font(font_3, time_0, 1097, 210, color_1);
        draw_font(font_2, title, 1130, 260, color_1);
    }




    //4.渲染呈现
//    SDL_RenderPresent(rdr);
}


//事件监听
int core() {
    SDL_Event event;
    SDL_Point pt;


    skip:
        draw_background();
        draw_day(days);
        SDL_RenderPresent(rdr);
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
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
                case SDL_MOUSEBUTTONDOWN:
                    //鼠标按下坐标event.button.x   event.button.y
                    //鼠标按下索引event.button.button
                    //鼠标点击次数event.button.clicks
                    pt = {event.button.x, event.button.y};
                    SDL_Rect rect{1049, 199, 391, 102};
                    if (SDL_PointInRect(&pt, &rect_1)) {
                        journal = journal_record();

                    } else if (SDL_PointInRect(&pt, &rect) && strlen(journal) >= 4) {
                        draw_journal();
                    } else if (SDL_PointInRect(&pt, &rect_2) && strlen(journal) >= 4) {
                        //3.设置渲染颜色
                        SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);

                        //4.清除屏幕(渲染屏幕背景)
                        SDL_RenderClear(rdr);
                        SDL_RenderPresent(rdr);

                        int x, y;
                        x = atoi(text_input(650, 0, 5));
                        y = atoi(text_input(580, 70, 6));
                        deleteLineFromFile("journal.txt", x, y);
                    } else if (SDL_PointInRect(&pt, &rect_one)) {
                        setalarm();
                    }
                    goto skip;
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
void deinit() {


    cleanup_img_texture();
    SDL_FreeSurface(txt_surf);
    SDL_DestroyTexture(txt_texture);
    SDL_DestroyTexture(img_texture);
    TTF_CloseFont(font_1);
    TTF_CloseFont(font_2);

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


//播放声音
void play_wav() {
    SDL_AudioSpec audio_spec;
    //1.导入WAV文件
    //SDL_LoadWAV('文件名',音频空间地址,音频数据地址,音频字节地址);
    SDL_LoadWAV("ring.wav", &audio_spec, &audio_buf, &audio_len);


    //2.定义播放回调函数
    audio_spec.callback = callback;

    //3.打开音频设备
    device_id = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);

    //4.开始播放
    SDL_PauseAudioDevice(device_id, 0);
    event_loop();
    //5.释放资源 关闭设备
    SDL_FreeWAV(audio_buf);
    SDL_CloseAudioDevice(device_id);
}

//显示日期文字
void draw_day(char *days[35]) {
    //3.渲染字体
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

    //7.释放与销毁资源


}

void draw_time() {
    time_t current_time = time(NULL);
    struct tm *timeinfo = localtime(&current_time);
    char time_string[9];
    strftime(time_string, 9, "%H:%M:%S", timeinfo);
    draw_font(font_1, time_string, 750, 30, color_1);

}

void draw_font(TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    txt_surf = TTF_RenderUTF8_Blended(font, text, {color});
    txt_texture = SDL_CreateTextureFromSurface(rdr, txt_surf);
    SDL_Rect txt_rect{x, y, txt_surf->w, txt_surf->h};
    SDL_RenderCopy(rdr, txt_texture, NULL, &txt_rect);
}


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

char *journal_record() {
    //1.渲染背景
    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
    SDL_RenderClear(rdr);
    draw_img();
    SDL_RenderPresent(rdr);
    //打开文件记录日志
    FILE *file = fopen("journal.txt", "a+");
    if (file == NULL) {
        printf("Failed to open file.\n");
    }

    //2.记录时间与标题
    time_0 = text_input(520, 0, 3);
    title = text_input(330, 70, 4);
    fprintf(file, "时间:%s\n", time_0);
    fprintf(file, "标题:%s\n", title);
    //3.重新渲染背景进行输入
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
                        fprintf(file, "内容:%s\n", text);
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

// 全局变量或者存储在某个合适的数据结构中

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

void draw_journal() {
    char *text = readEntireFile("journal.txt");
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
//                case SDL_MOUSEBUTTONDOWN:
//                    pt = {event.button.x, event.button.y};
//                    SDL_Rect rect{1049, 199, 391, 400};
//                    if (SDL_PointInRect(&pt, &rect)) {
//                        return;
//                    }
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
    printf("Year: %d\n", alarmTime->tm_year + 1900);
    printf("Month: %d\n", alarmTime->tm_mon + 1);
    printf("Day: %d\n", alarmTime->tm_mday);
    printf("Hour: %d\n", alarmTime->tm_hour);
    printf("Minute: %d\n", alarmTime->tm_min);
    printf("Second: %d\n", alarmTime->tm_sec);
    time_t alarm = mktime(alarmTime);
    time_t currentTime;

//    do {
//        time(&currentTime);
//    } while (difftime(alarm, currentTime) > 0);
    time(&currentTime);
    printf("Current time: %ld\n", currentTime);
    printf("set time: %ld\n", alarm);
    double time = difftime(alarm, currentTime);
    printf("%f", time);
    sleep(time);
    play_wav();
    pthread_exit(NULL);
}

void setalarm() {
    //3.设置渲染颜色
    SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);

    //4.清除屏幕(渲染屏幕背景)
    SDL_RenderClear(rdr);
    SDL_RenderPresent(rdr);

    int year_1, month_1, day, hours, minutes;
    pthread_t tid;
    pthread_attr_t attr;
    year_1 = atoi(text_input(320, 0, 7));
    month_1 = atoi(text_input(320, 70, 8));
    day = atoi(text_input(300, 140, 9));
    hours = atoi(text_input(300, 210, 10));
    minutes = atoi(text_input(300, 280, 11));
    struct tm alarmTime = {};
    alarmTime.tm_year = year_1 - 1900;
    alarmTime.tm_mon = month_1 - 1;
    alarmTime.tm_mday = day;
    alarmTime.tm_hour = hours;
    alarmTime.tm_min = minutes;
    alarmTime.tm_sec = 0;

    // 初始化线程属性对象
    pthread_attr_init(&attr);
    // 设置线程属性为分离状态
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    // 创建线程并传递线程属性对象
    pthread_create(&tid, &attr, alarmClock, (void *) &alarmTime);
    sleep(1);

}
