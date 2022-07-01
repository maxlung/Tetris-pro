#include <SFML/Graphics.hpp>
#include <time.h>
#include<iostream>
#include<fstream>
using namespace sf;
using namespace std;

const int M = 20; //場地高20格
const int N = 10; //寬10格
const int Blocksize = 27; //一個格子邊長27像素
int field[M][N] = { 0 }; //宣告場地(包括畫面上方)
int score = 0; //分數
int nextShape;

enum Goal { Bar = 0, Bigsqure, twoSqure, rvTwosqure, verticalBar }; //列舉不同種類的遊戲目標
int goalType = 0;

struct Point { //方塊struct,紀錄x跟y的位置
    int x, y;
}now[4], past[4], nextblock[4];


int shape[7][4] =  //用二維陣列記錄不同類型的方塊形狀
{
    1,3,5,7,//直線型
    2,4,5,7,//Z型
    3,5,4,6,//S型
    3,5,4,7,//凸字形
    2,3,5,7,//L
    3,5,6,7,//反L
    2,3,4,5,//方塊
};



bool Check() {//檢查是否越界
    for (int i = 0; i < 4; i++) {
        if (now[i].x < 0 || now[i].x >= N || now[i].y >= M) {//超出場地
            return 0;
        }else if(now[i].y>=0)
        if (field[now[i].y][now[i].x]) {//場地上已經有方塊
            return 0;
        }
    }
    return 1;
}

bool deadCheck() {       ///死亡判斷
    if (now[3].y == 1)
    if (field[1][now[3].x] != 0)
        return 1;

    if (now[2].y == 1)
    if (field[1][now[2].x] != 0)
        return 1;

    return 0;
}

void display(int color,Sprite s, RenderWindow &window) {     
    
    //畫出場地邊框
    RectangleShape line(sf::Vector2f(5, Blocksize * 20+5)), line2(sf::Vector2f(Blocksize * 10, 5));
    line.setPosition(Blocksize * 10, 0);  line2.setPosition(0, Blocksize * 20);
    line.setFillColor(Color::Black); line2.setFillColor(Color::Black);
    window.draw(line);
    window.draw(line2);

    //印出場上方塊
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] != 0) {
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * Blocksize, i * Blocksize);    
                window.draw(s);
            }
        }
    }
    //印出正在控制的方塊
    for (int i = 0; i < 4; i++) {
        s.setTextureRect(IntRect(color * 18, 0, 18, 18));
        if (now[i].y >= 0) {
            s.setPosition(now[i].x * Blocksize, now[i].y * Blocksize);
            window.draw(s);
        }
    }
    //印出下一個方塊
    for (int i = 0; i < 4; i++) {
        s.setTextureRect(IntRect(color * 18, 0, 18, 18));
        s.setPosition((nextblock[i].x+10) * Blocksize, (nextblock[i].y+4) * Blocksize);
        window.draw(s);
    }
    //用switch印出目標消除方塊
    switch (goalType)
    {
    case Bar :
        for (int i = 0; i < 10; i++) {
            s.setTextureRect(IntRect(color * 18, 0, 18, 18));
            s.setPosition( (7+i)* Blocksize, 25 * Blocksize);
            window.draw(s);
        }
        break;
    case Bigsqure:
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                s.setTextureRect(IntRect(color * 18, 0, 18, 18));
                s.setPosition((13 - i) * Blocksize, (24-j) * Blocksize);
                window.draw(s);
            }
        }
        break;
    case twoSqure:
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                s.setTextureRect(IntRect(color * 18, 0, 18, 18));
                if (i + j > 0 && i + j!= 4)
                {
                    int k=i;
                    if (i + j == 2) {
                        if (i == 0)k = 2;
                        if (i == 2)k = 0;
                    }
                    s.setPosition((13 - k) * Blocksize, (24 - j) * Blocksize);
                    window.draw(s);
                }
            }

        }
        break;
    case rvTwosqure:
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                s.setTextureRect(IntRect(color * 18, 0, 18, 18));
                if (i + j > 0 && i + j != 4)
                {
                    s.setPosition((13 - i) * Blocksize, (24 - j) * Blocksize);
                    window.draw(s);
                }
            }

        }
        break;
    case verticalBar:
        for (int i = 0; i < 10; i++) {
            s.setTextureRect(IntRect(color * 18, 0, 18, 18));
            s.setPosition(15  * Blocksize, (15 + i) * Blocksize);
            window.draw(s);
        }
        break;
    }

    window.display();
}
void printScoreAndText(Font font, int score, RenderWindow& window) {//印出畫面上的文字以及顯示分數
    string str = "your score \n              ";
    string scorestr = to_string(score);
    str.append(scorestr);
    Text text;
    Text Goal;
    Text nextblock;
    text.setFont(font);
    text.setString(str);
    text.setFillColor(Color::Black);
    text.setCharacterSize(36);
    text.setPosition(5, 550);
    window.draw(text);
    Goal.setFont(font);
    Goal.setString("Goal\n");
    Goal.setFillColor(Color::Red);
    Goal.setCharacterSize(30);
    Goal.setPosition(310, 540);
    window.draw(Goal);
    nextblock.setFont(font);
    nextblock.setString("Next\n");
    nextblock.setFillColor(Color::Black);
    nextblock.setCharacterSize(30);
    nextblock.setPosition(350, 45);
    window.draw(nextblock);
}

void overdisplay(Font font, int score, RenderWindow& window,bool &isPlaying) {
    while (window.isOpen()&&!isPlaying) {
        Event ev;
        window.clear(Color::White);
        //設定顯示的字串
        string str = "  Gameover!!\n\n your score is             \n\n          ";
        string scorestr = to_string(score);
        str.append(scorestr);
        Text text;
        Text over;
        text.setFont(font);
        text.setString(str);
        text.setFillColor(Color::Red);
        text.setCharacterSize(40);
        text.setPosition(90, 250);

        window.draw(text);
        window.display();
        while (window.pollEvent(ev)) //取得事件的迴圈
        {
            if (ev.type == Event::Closed)//若事件是關閉按鈕就關閉視窗
                window.close();
            if (ev.type == Event::KeyPressed) {
                if (ev.key.code == Keyboard::R) {//按r會繼續遊玩
                    isPlaying = true;
                }

            }
            
        }
    }
    
}
//消除水平長條
void horizontalBar(){
    int k = M - 1;
    bool success = 0;
    for (int i = M - 1; i > 0; i--) {
        int count = 0;
        for (int j = 0; j < N; j++) {
            if (field[i][j])
                count++;
            field[k][j] = field[i][j];
        }
        if (count < N) k--;
        if (count == N) {
            score += 1000;
            success = 1;
        }
    }
    if (success)goalType = rand() % 5;
}

//消除3*3方塊
void bigsqure() {
    for (int i = M - 1; i >= 2;i--) {
        for (int j = N - 1; j >= 2;j--) {//逐格檢查
            int count = 0;
            for (int k = 0; k < 3; k++) 
            for (int n = 0; n < 3; n++){
                if (field[i-k][j-n] != 0)count++;

            }
            if (count == 9) {
                goalType = rand() % 5;
                score += 1000;
                for (int k = 0; k < 3; k++)
                for (int n = 0; n < 3; n++){
                    field[i - k][j - n] = field[i - k - 3][j - n];
                    field[i - k - 3][j - n] = 0;
                
                }
                
            }

        }
    }
}

//消除2*2+2*2圖形
void twosqure() {
    for (int i = M - 1; i >= 2; i--) {
        for (int j = N - 1; j >= 2; j--) {//逐格檢查
            int count = 0;
            for (int k = 0; k < 3; k++)
            for (int n = 0; n < 3; n++) {
                if (field[i - k][j - n] != 0)count++;

            }
            if (field[i - 2][j-2] != 0)count--;
            if (field[i][j ] != 0)count--;

            if (count >= 7) {
                goalType = rand() % 5;
                score += 1500;
                for (int k = 0; k < 3; k++)
                for (int n = 0; n < 3; n++) {
                    if ((k != 2 && n != 2) || (n != 0 && k != 0)) {
                        field[i - k][j - n] = 0;

                    }
                }

            }

        }
    }
}

//消除反的2*2+2*2圖形
void rvtwosqure() {
    for (int i = M - 1; i >= 2; i--) {
        for (int j = N - 1; j >= 2; j--) {//逐格檢查
            int count = 0;
            for (int k = 0; k < 3; k++)
            for (int n = 0; n < 3; n++) {
                if (field[i - k][j - n] != 0)count++;

            }
            if (field[i - 2][j] != 0)count--;
            if (field[i ][j - 2] != 0)count--;

            if (count >= 7) {
                goalType = rand() % 5;
                score += 1500;
                for (int k = 0; k < 3; k++)
                for (int n = 0; n < 3; n++) {
                    if ((k!=2&&n!=0)||(n!=2&&k!=0)) {
                        field[i - k][j - n] = 0;
                        
                    }
                }

            }

        }
    }
}

//消除垂直長條
void verticalbar() {

    for (int i = N - 1; i > 0; i--) {
        for (int j = M - 1; j > 10; j--) {
            int count = 0;
            for (int n = 0; n < 10; n++) {
                if (field[j - n][i])count++;

            }
            if (count == 10) {
                goalType = rand() % 5;
                score += 1000;
                for (int n = 0; n < 10; n++) {
                    field[j - n][i] = 0;

                }
            }
        }
        
    }
}


void saveScore(int score) {
    fstream f;
    int line = 1;
    string tmps;
    f.open("score.txt", std::ios_base::in);//開檔讀取
    while (getline(f, tmps)) { //計算是第幾次嘗試
        line++;//一行一行讀，每讀一行就加一
    }
    f.close();

    f.open("score.txt", std::ios_base::app);
    if (!f.is_open()) {
        cout << "Failed to open file.\n";
    }
    else {
        f << "第" << line << "次嘗試 分數:" << score << endl;//寫入分數
    }

}

int main(){
    srand(time(NULL));
    bool isGame = true, isPlaying =true;
    Font font;
    if (!font.loadFromFile("arial.ttf"))//讀取字體
    {
        cout << "error";
    }

    RenderWindow window(VideoMode(480, 720), "MyGame");//宣告視窗
    Texture t;
    t.loadFromFile("images/blocks.png");//讀取方塊圖片
    Sprite s(t);//宣告方塊的sprite
    s.setTextureRect(IntRect(0,0, 18, 18));//擷取素材的一部分
    s.Transformable::setScale(1.5 , 1.5);//設定方塊大小


    int dx = 0, color = 1; //紀錄是否水平位移  記錄當前方塊顏色 
    bool rotation = false; //方塊旋轉狀態
    float timer = 0 , delay = 0.3; //計時器與下降的時間差

    Clock clock;

    int n = rand() % 7;//隨機產生第一個方塊形狀
    nextShape = rand() % 7;//隨機產生下一個的方塊形狀
    for (int i = 0; i < 4; i++) {
        now[i].x = shape[n][i] % 2 + 3;
        now[i].y = shape[n][i] / 2-3;
    }
    for (int i = 0; i < 4; i++) {
        nextblock[i].x = shape[nextShape][i] % 2 + 3;
        nextblock[i].y = shape[nextShape][i] / 2;
    }

    while (isGame) {
        Event ev;
        if(isPlaying)
            while (window.isOpen())
            {
                float time = clock.getElapsedTime().asSeconds();
                clock.restart();
                timer += time;

                Event ev;
                while (window.pollEvent(ev)) //取得事件的迴圈
                {
                    if (ev.type == Event::Closed)//若事件是關閉按鈕就關閉視窗
                        window.close();

                    if (ev.type == Event::KeyPressed)
                        if (ev.key.code == Keyboard::Up) {//上是旋轉
                            rotation = true;
                        }
                        else if (ev.key.code == Keyboard::Left) {//左是向左
                            dx = -1;
                        }
                        else if (ev.key.code == Keyboard::Right) {//右是向右
                            dx = 1;
                        }
                        else if (ev.key.code == Keyboard::Down) {//下是加速下降
                            delay = 0.05; //讓下降的間隔時間減少
                        }

                }

                // move
                for (int i = 0; i < 4; i++) {
                    past[i] = now[i];
                    now[i].x += dx;
                }
                if (!Check()) {//越界檢查
                    for (int i = 0; i < 4; i++) {
                        now[i] = past[i];
                    }
                }

                // rotation
                if (rotation) {
                    Point center = now[1];//旋轉的中心
                    for (int i = 0; i < 4; i++) {
                        int x = now[i].y - center.y;//y座標跟中心y座標的距離
                        int y = now[i].x - center.x;//x座標跟中心x座標的距離
                        now[i].x = center.x - x;//從中心x座標開始走一樣距離
                        now[i].y = center.y + y;//從中心y座標開始走一樣距離
                    }
                    if (!Check()) {
                        for (int i = 0; i < 4; i++) {
                            now[i] = past[i];
                        }
                    }
                }
                //下降
                if (timer > delay) {
                    for (int i = 0; i < 4; i++) {
                        past[i] = now[i];
                        now[i].y++;
                    }
                    ///死亡判定
                    if (deadCheck()) {
                        isPlaying = false;//狀態變成不在遊戲中
                        saveScore(score);//將分數儲存到txt檔案中
                        break;//跳出遊戲迴圈
                        
                    }

                    if (now[0].y >= 0)
                        if (!Check()) {//如果碰到底部就要將方塊記錄到場地上，並產生下一個方塊
                            for (int i = 0; i < 4; i++) {
                                field[past[i].y][past[i].x] = color;//將場地上的格子標記方塊顏色
                            }

                            color = 1 + rand() % 7;//隨機產生新的顏色
                            cout << past[0].y << endl;
                            //設定方塊為預定好的下一個方塊
                            int n = nextShape;
                            for (int i = 0; i < 4; i++) {
                                now[i].x = shape[n][i] % 2 + 3;
                                now[i].y = (shape[n][i] / 2) - 3;
                            }

                            nextShape = rand() % 7;//隨機產生下一個「下一個的方塊」的形狀
                            for (int i = 0; i < 4; i++) {
                                nextblock[i].x = shape[nextShape][i] % 2 + 3;
                                nextblock[i].y = shape[nextShape][i] / 2;
                            }

                        }
                    timer = 0;//重製計時器
                }


                ///消除目標

                switch (goalType)
                {
                case Bar://水平長條
                    horizontalBar();
                    break;
                case Bigsqure://3*3方塊
                    bigsqure();
                    break;
                case twoSqure://兩個小方塊
                    twosqure();
                    break;
                case rvTwosqure://反的兩個小方塊
                    rvtwosqure();
                    break;
                case verticalBar://垂直長條
                    verticalbar();
                    break;
                }

                //重置方塊狀態//
                dx = 0; rotation = false; delay = 0.3;

                ///更新畫面///
                window.clear(Color::White);

                printScoreAndText(font, score, window);//顯示分數及文字
                display(color, s, window);//顯示方塊位置
            }
            
            /// 清除場上的方塊與分數、重製遊戲目標(為了下一次重新開始)
            for (int i = 0; i < M; i++) 
                for (int j = 0; j < N; j++)
                    field[i][j] = 0;
            score = 0;
            goalType = 0;
        /// 遊戲結束畫面(可以按R重新開始)
 
        overdisplay(font, score, window,isPlaying);
    }
    
    return 0; 
}

