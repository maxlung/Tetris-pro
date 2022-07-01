#include <SFML/Graphics.hpp>
#include <time.h>
#include<iostream>
#include<fstream>
using namespace sf;
using namespace std;

const int M = 20; //���a��20��
const int N = 10; //�e10��
const int Blocksize = 27; //�@�Ӯ�l���27����
int field[M][N] = { 0 }; //�ŧi���a(�]�A�e���W��)
int score = 0; //����
int nextShape;

enum Goal { Bar = 0, Bigsqure, twoSqure, rvTwosqure, verticalBar }; //�C�|���P�������C���ؼ�
int goalType = 0;

struct Point { //���struct,����x��y����m
    int x, y;
}now[4], past[4], nextblock[4];


int shape[7][4] =  //�ΤG���}�C�O�����P����������Ϊ�
{
    1,3,5,7,//���u��
    2,4,5,7,//Z��
    3,5,4,6,//S��
    3,5,4,7,//�Y�r��
    2,3,5,7,//L
    3,5,6,7,//��L
    2,3,4,5,//���
};



bool Check() {//�ˬd�O�_�V��
    for (int i = 0; i < 4; i++) {
        if (now[i].x < 0 || now[i].x >= N || now[i].y >= M) {//�W�X���a
            return 0;
        }else if(now[i].y>=0)
        if (field[now[i].y][now[i].x]) {//���a�W�w�g�����
            return 0;
        }
    }
    return 1;
}

bool deadCheck() {       ///���`�P�_
    if (now[3].y == 1)
    if (field[1][now[3].x] != 0)
        return 1;

    if (now[2].y == 1)
    if (field[1][now[2].x] != 0)
        return 1;

    return 0;
}

void display(int color,Sprite s, RenderWindow &window) {     
    
    //�e�X���a���
    RectangleShape line(sf::Vector2f(5, Blocksize * 20+5)), line2(sf::Vector2f(Blocksize * 10, 5));
    line.setPosition(Blocksize * 10, 0);  line2.setPosition(0, Blocksize * 20);
    line.setFillColor(Color::Black); line2.setFillColor(Color::Black);
    window.draw(line);
    window.draw(line2);

    //�L�X���W���
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] != 0) {
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * Blocksize, i * Blocksize);    
                window.draw(s);
            }
        }
    }
    //�L�X���b������
    for (int i = 0; i < 4; i++) {
        s.setTextureRect(IntRect(color * 18, 0, 18, 18));
        if (now[i].y >= 0) {
            s.setPosition(now[i].x * Blocksize, now[i].y * Blocksize);
            window.draw(s);
        }
    }
    //�L�X�U�@�Ӥ��
    for (int i = 0; i < 4; i++) {
        s.setTextureRect(IntRect(color * 18, 0, 18, 18));
        s.setPosition((nextblock[i].x+10) * Blocksize, (nextblock[i].y+4) * Blocksize);
        window.draw(s);
    }
    //��switch�L�X�ؼЮ������
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
void printScoreAndText(Font font, int score, RenderWindow& window) {//�L�X�e���W����r�H����ܤ���
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
        //�]�w��ܪ��r��
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
        while (window.pollEvent(ev)) //���o�ƥ󪺰j��
        {
            if (ev.type == Event::Closed)//�Y�ƥ�O�������s�N��������
                window.close();
            if (ev.type == Event::KeyPressed) {
                if (ev.key.code == Keyboard::R) {//��r�|�~��C��
                    isPlaying = true;
                }

            }
            
        }
    }
    
}
//������������
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

//����3*3���
void bigsqure() {
    for (int i = M - 1; i >= 2;i--) {
        for (int j = N - 1; j >= 2;j--) {//�v���ˬd
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

//����2*2+2*2�ϧ�
void twosqure() {
    for (int i = M - 1; i >= 2; i--) {
        for (int j = N - 1; j >= 2; j--) {//�v���ˬd
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

//�����Ϫ�2*2+2*2�ϧ�
void rvtwosqure() {
    for (int i = M - 1; i >= 2; i--) {
        for (int j = N - 1; j >= 2; j--) {//�v���ˬd
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

//������������
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
    f.open("score.txt", std::ios_base::in);//�}��Ū��
    while (getline(f, tmps)) { //�p��O�ĴX������
        line++;//�@��@��Ū�A�CŪ�@��N�[�@
    }
    f.close();

    f.open("score.txt", std::ios_base::app);
    if (!f.is_open()) {
        cout << "Failed to open file.\n";
    }
    else {
        f << "��" << line << "������ ����:" << score << endl;//�g�J����
    }

}

int main(){
    srand(time(NULL));
    bool isGame = true, isPlaying =true;
    Font font;
    if (!font.loadFromFile("arial.ttf"))//Ū���r��
    {
        cout << "error";
    }

    RenderWindow window(VideoMode(480, 720), "MyGame");//�ŧi����
    Texture t;
    t.loadFromFile("images/blocks.png");//Ū������Ϥ�
    Sprite s(t);//�ŧi�����sprite
    s.setTextureRect(IntRect(0,0, 18, 18));//�^���������@����
    s.Transformable::setScale(1.5 , 1.5);//�]�w����j�p


    int dx = 0, color = 1; //�����O�_�����첾  �O����e����C�� 
    bool rotation = false; //������બ�A
    float timer = 0 , delay = 0.3; //�p�ɾ��P�U�����ɶ��t

    Clock clock;

    int n = rand() % 7;//�H�����ͲĤ@�Ӥ���Ϊ�
    nextShape = rand() % 7;//�H�����ͤU�@�Ӫ�����Ϊ�
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
                while (window.pollEvent(ev)) //���o�ƥ󪺰j��
                {
                    if (ev.type == Event::Closed)//�Y�ƥ�O�������s�N��������
                        window.close();

                    if (ev.type == Event::KeyPressed)
                        if (ev.key.code == Keyboard::Up) {//�W�O����
                            rotation = true;
                        }
                        else if (ev.key.code == Keyboard::Left) {//���O�V��
                            dx = -1;
                        }
                        else if (ev.key.code == Keyboard::Right) {//�k�O�V�k
                            dx = 1;
                        }
                        else if (ev.key.code == Keyboard::Down) {//�U�O�[�t�U��
                            delay = 0.05; //���U�������j�ɶ����
                        }

                }

                // move
                for (int i = 0; i < 4; i++) {
                    past[i] = now[i];
                    now[i].x += dx;
                }
                if (!Check()) {//�V���ˬd
                    for (int i = 0; i < 4; i++) {
                        now[i] = past[i];
                    }
                }

                // rotation
                if (rotation) {
                    Point center = now[1];//���઺����
                    for (int i = 0; i < 4; i++) {
                        int x = now[i].y - center.y;//y�y�и򤤤�y�y�Ъ��Z��
                        int y = now[i].x - center.x;//x�y�и򤤤�x�y�Ъ��Z��
                        now[i].x = center.x - x;//�q����x�y�ж}�l���@�˶Z��
                        now[i].y = center.y + y;//�q����y�y�ж}�l���@�˶Z��
                    }
                    if (!Check()) {
                        for (int i = 0; i < 4; i++) {
                            now[i] = past[i];
                        }
                    }
                }
                //�U��
                if (timer > delay) {
                    for (int i = 0; i < 4; i++) {
                        past[i] = now[i];
                        now[i].y++;
                    }
                    ///���`�P�w
                    if (deadCheck()) {
                        isPlaying = false;//���A�ܦ����b�C����
                        saveScore(score);//�N�����x�s��txt�ɮפ�
                        break;//���X�C���j��
                        
                    }

                    if (now[0].y >= 0)
                        if (!Check()) {//�p�G�I�쩳���N�n�N����O������a�W�A�ò��ͤU�@�Ӥ��
                            for (int i = 0; i < 4; i++) {
                                field[past[i].y][past[i].x] = color;//�N���a�W����l�аO����C��
                            }

                            color = 1 + rand() % 7;//�H�����ͷs���C��
                            cout << past[0].y << endl;
                            //�]�w������w�w�n���U�@�Ӥ��
                            int n = nextShape;
                            for (int i = 0; i < 4; i++) {
                                now[i].x = shape[n][i] % 2 + 3;
                                now[i].y = (shape[n][i] / 2) - 3;
                            }

                            nextShape = rand() % 7;//�H�����ͤU�@�ӡu�U�@�Ӫ�����v���Ϊ�
                            for (int i = 0; i < 4; i++) {
                                nextblock[i].x = shape[nextShape][i] % 2 + 3;
                                nextblock[i].y = shape[nextShape][i] / 2;
                            }

                        }
                    timer = 0;//���s�p�ɾ�
                }


                ///�����ؼ�

                switch (goalType)
                {
                case Bar://��������
                    horizontalBar();
                    break;
                case Bigsqure://3*3���
                    bigsqure();
                    break;
                case twoSqure://��Ӥp���
                    twosqure();
                    break;
                case rvTwosqure://�Ϫ���Ӥp���
                    rvtwosqure();
                    break;
                case verticalBar://��������
                    verticalbar();
                    break;
                }

                //���m������A//
                dx = 0; rotation = false; delay = 0.3;

                ///��s�e��///
                window.clear(Color::White);

                printScoreAndText(font, score, window);//��ܤ��ƤΤ�r
                display(color, s, window);//��ܤ����m
            }
            
            /// �M�����W������P���ơB���s�C���ؼ�(���F�U�@�����s�}�l)
            for (int i = 0; i < M; i++) 
                for (int j = 0; j < N; j++)
                    field[i][j] = 0;
            score = 0;
            goalType = 0;
        /// �C�������e��(�i�H��R���s�}�l)
 
        overdisplay(font, score, window,isPlaying);
    }
    
    return 0; 
}

