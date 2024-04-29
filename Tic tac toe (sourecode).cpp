#include <iostream> // thu vien ho tro nhap xuat co ban cho chuong trinh game
#include <stdexcept> //thu vien ho tro thanh phan kiem tra cac nuoc di loi (computer)
#include <cstdlib> //thu vien ho tro tao ra cac thanh phan an phim (toa do) de thao tac khi choi
#include <ctime> // thu vien ho tro tao ra cac buoc di ngau nhien cua computer
#include <windows.h> // thu vien ho tro color

using namespace std;
// Xay dung class Board de thiet ke khung gameplay cua tro choi
class Board {
    char arr[3][3]; // Khoi tao mang 2 chieu kiem soat board game
public:
	// Khoi tao Board game
    Board() {
        clearBoard();
    }
    // Ham bo tro thiet ke va quan ly su thay doi cua gameplay
    // Ham clearBoard su dung de xoa noi dung cua bang choi bang cach gan moi o cua bang thanh dau gach ngang (“-”). 
    void clearBoard() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                arr[i][j] = '-';
            }
        }
    }
    // Ham printBoard su dung de in ra noi dung cua bang choi, hien thi bang duoi dang ma tran 3x3 voi cac dau gach ngang va cac ky tu (X, O hoac -) tuong ung
    void printBoard() {
    	Set_colorBoard(4);
		cout << "X ";
    	Set_colorBoard(2);
    	cout << "===========================================";
    	Set_colorBoard(4);
		cout << " X";
		cout << endl;
		Set_colorBoard(7);
    	cout << "  -------------------------------------------" << endl;
    	for (int i = 0; i < 3; ++i) {
        	cout << "  | ";
        	for (int j = 0; j < 3; ++j) {
            	cout << arr[i][j] << "           | ";
        	}
        	cout << endl;
        	cout << "  -------------------------------------------" << endl;
    	}
    	Set_colorBoard(4);
		cout << "O ";
    	Set_colorBoard(2);
    	cout << "===========================================";
		Set_colorBoard(4);
		cout << " O" ;
		cout << endl;
	}
	//Ham placeMarker duoc su dung de dat mot ky tu (danh dau) vao o cua bang voi vi tri duoc chi dinh boi hang row va cot col
    bool placeMarker(int row, int col, char marker) {
        if (row < 0 || row >= 3 || col < 0 || col >= 3 || arr[row][col] != '-') {
            return false;
        }
        arr[row][col] = marker;
        return true;
    }
	// Ham checkWinner duoc su dung de kiem tra xem co nguoi chien thang nao khong sau moi luot danh.
    char checkWinner() {
        for (int i = 0; i < 3; ++i) {
            if (arr[i][0] != '-' && arr[i][0] == arr[i][1] && arr[i][1] == arr[i][2]) {
                return arr[i][0];
            }
        }
        for (int i = 0; i < 3; ++i) {
            if (arr[0][i] != '-' && arr[0][i] == arr[1][i] && arr[1][i] == arr[2][i]) {
                return arr[0][i];
            }
        }
        if (arr[0][0] != '-' && arr[0][0] == arr[1][1] && arr[1][1] == arr[2][2]) {
            return arr[0][0];
        }
        if (arr[0][2] != '-' && arr[0][2] == arr[1][1] && arr[1][1] == arr[2][0]) {
            return arr[0][2];
        }
        return '-';
    }
	// full duoc su dung de kiem tra xem bang da day chua. 
    bool full() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (arr[i][j] == '-') {
                    return false;
                }
            }
        }
        return true;
    }
    void Set_colorBoard(int color) {
   		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    	SetConsoleTextAttribute(hConsole, color);
	}
};
// Xay dung class Player de xac dinh nguoi choi la human hoac bot
class Player {
protected:
    char marker;
public:
	// Khoi tao
    Player(char m) : marker(m) {} 
    virtual void makeMove(Board& board) = 0;// Ham ao, duoc trien khai trong cac lop con nhu HumanPlayer va ComputerPlayer de thuc hien cac nuoc di cua nguoi choi.
};
// Xay dung class HumanPlayer ke thua tu lop Player
class HumanPlayer : public Player {
public:
	// Khoi tao
    HumanPlayer(char m) : Player(m) {}
    // Ham bo tro
    //  Ham makeMove yeu cau nguoi choi con nguoi nhap hang va cot de danh dau tren bang. No kiem tra tinh hop le cua nuoc di va neu khong hop le, no se nem mot ngoai le (exception). 
    void makeMove(Board& board) override { 
        cout << "Enter your move (1 -> 9): ";
        int pos;
        cin >> pos;
        int row = (pos - 1) / 3;
    	int col = (pos - 1) % 3;
        if (pos < 1 || pos > 9 || !board.placeMarker(row, col, marker)) {
        	throw invalid_argument("INVALID MOVE!");
    	}
    }
};
//***Xay dung class ComputerPlayer ke thua tu lop Player***//
class ComputerPlayer : public Player {
public:
	// Khoi tao
    ComputerPlayer(char m) : Player(m) {}
    // Ham bo tro
    // Ham makeMove tao ra mot nuoc di ngau nhien cho nguoi choi may tinh bang cach chon mot hang va cot ngau nhien tren bang va dat ky tu vao o do. Neu o da duoc danh dau, no se chon mot o khac. 
    void makeMove(Board& board) override {
        int row, col;
        do {
            row = rand() % 3;
            col = rand() % 3;
        } while (!board.placeMarker(row, col, marker));
    }
};
//***Xay dung class Game de quan ly che do cua game***//
class Game {
    Board board;
    Player* player1;
    Player* player2;
    
    void switchPlayers(Player*& current) {
        if (current == player1) {
            current = player2;
        } else {
            current = player1;
        }
    }

public:
	// Khoi tao
    Game(Player* p1, Player* p2) : player1(p1), player2(p2) {}
	// Ham bo tro
	// Ham play dieu khien luong choi cua tro choi Tic Tac Toe. No lap lai cac luot choi cho den khi co mot nguoi chien thang hoac o trong day. 
    void play() {
        Player* current = player1;
        while (1) {
        	Set_colorGame(4);
            cout << "Tic Tac Toe ";
            Set_colorGame(7);
			cout << "3x3 Gameplay" << endl;
            board.printBoard();
            cout << endl;

            try {
                current->makeMove(board);
            } catch (const invalid_argument& e) {
            	Set_colorGame(4);
                cerr << "Error: " << e.what() << endl;
                continue;
            }

            char winner = board.checkWinner();
            if (winner != '-') {
            	Set_colorGame(6);
                cout << "Victory is: " << winner << endl;
                board.printBoard();
                break;
            } else if (board.full()) {
            	Set_colorGame(6);
                cout << "TIE!" << endl;
                board.printBoard(); 
                break;
            }

            switchPlayers(current);
        }
    }
    // xay dung color chu trong class Game
    void Set_colorGame(int color) {
    	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    	SetConsoleTextAttribute(hConsole, color);
	}
};
// xay dung color font chu
void Set_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
// xay dung mau nen (hightlight) cua font chu
void Set_background_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color << 4);
}
// build giao dien gameloop(khong lap man hinh intro)
void clearScreen() {
    system("cls");
}
//build intro game
void showIntrodaugame() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||      /\\------------/\\            ___________           ||" << endl;
    cout << "||     |    @       @   |           // _______//          ||" << endl;
    cout << "||     |        .       |          | /_________           ||" << endl;
    cout << "||     |  =     ^    =  |          |    _____  \\          ||" << endl;
    cout << "||      ----------------           |   (_____) ||         ||" << endl;
    cout << "||      /-------0------\\           \\__________||          ||" << endl;
    cout << "||     /   =()[Dev]()=  \\             5TUD10              ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showOutro0() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                   Quitting game.                       ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |                                      | 0%     ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showOutro10() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                   Quitting game..                      ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |====                                  | 10%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showOutro20() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                   Quitting game...                     ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |=========                             | 20%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showOutro30() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                   Quitting game                        ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |===============                       | 30%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showOutro40() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                   Quitting game.                       ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |==================                    | 40%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showOutro50() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                   Quitting game..                      ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |====================                  | 50%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showOutro60() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                   Quitting game...                     ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |=========================             | 60%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showOutro70() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                   Quitting game                        ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |==============================        | 70%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showOutro80() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                   Quitting game.                       ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |==================================    | 80%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showOutro95() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                   Quitting game..                      ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |====================================  | 95%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showOutro100() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                   Quitting game...                     ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |======================================| 100%   ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showIntro0() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                Get ready for some fun!                 ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |                                      | 0%     ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showIntro10() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                Get ready for some fun!                 ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |====                                  | 10%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showIntro20() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                Get ready for some fun!                 ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |=========                             | 20%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showIntro30() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                Get ready for some fun!                 ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |===============                       | 30%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showIntro40() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||                Get ready for some fun!                 ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |==================                    | 40%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showIntro50() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||            Note: Careful to tick the board :v          ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |====================                  | 50%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showIntro60() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||            Note: Careful to tick the board :v          ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |=========================             | 60%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showIntro70() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||            Note: Careful to tick the board :v          ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |==============================        | 70%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showIntro80() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||            Note: Careful to tick the board :v          ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |==================================    | 80%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showIntro95() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||            Note: Careful to tick the board :v          ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |====================================  | 95%    ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
void showIntro100() {
	Set_background_color(0);
	clearScreen();
    Set_color(2);
    cout << "================== [X] Tic Tac Toe [O] =====================" << endl;
    Set_color(3);
    cout << "||                                                        ||" << endl;
    cout << "||            Note: Careful to tick the board :v          ||" << endl;
    cout << "||          ______________________________________        ||" << endl;
    cout << "||        |======================================| 100%   ||" << endl;
    cout << "||          --------------------------------------        ||" << endl;
    cout << "||                                                        ||" << endl;
    cout << "||                                                        ||" << endl;
    Set_color(2);
    cout << "============================================================" << endl;
    cout << endl;
}
// build giao dien menu cua game
void Menu() {
	Set_background_color(0);
	Set_color(2);
    cout << "===========[X] Tic Tac Toe Game [O]============" << endl;
    Set_color(3);
    cout << "||          1. Play against Computer         ||" << endl;
    cout << "||          2. Play with a Friend            ||" << endl;
    cout << "||          3. Computer vs Computer          ||" << endl;
    cout << "||          4. How to play                   ||" << endl;
    cout << "||          5. Information                   ||" << endl;
    cout << "||          6. Exit Game                     ||" << endl;
    Set_color(2);
    cout << "===============================================" << endl;
    Set_background_color(0);
    cout << endl;
}

int main() {
    srand(time(NULL));
    char choice; // tao ra bien de lua chon cac muc trong menu gameplay
    showIntrodaugame();
    Sleep(1000);
    showIntro0();
    Sleep(50);
    showIntro10();
    Sleep(50);
    showIntro20();
    Sleep(50);
    showIntro30();
    Sleep(50);
    showIntro40();
    Sleep(50);
    showIntro50();
    Sleep(50);
    showIntro60();
    Sleep(1000);
    showIntro70();
    Sleep(50);
    showIntro80();
    Sleep(30);
    showIntro95();
    Sleep(1000);
    showIntro100();
    Sleep(30);
    clearScreen();
    while (1) {
    	Menu();
    	
    	Set_color(2);
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case '1': {
            	Set_color(4);
            	cout << "Human vs Computer"<< endl;
            	Set_color(7);
            	char mark1, mark2;
            	bool checkMarker1 = true;
            	bool checkMarker2 = true;
				cout << "Player 1(X/O): ";
				cin >> mark1;
            	while (checkMarker1){
            		if(mark1 != 'X' && mark1 != 'O'){
            			cout << "Invalid marker! Please choose marker again player 1: ";
            			cin >> mark1;
					} else {
						break;
					} 
				}
				cout << "Player 2(X/O): ";
				cin >> mark2;
				while (checkMarker2){
            		if(mark2 != 'X' && mark2 != 'O' || mark2 == mark1){
            			cout << "Invalid marker! Please choose marker again player 2: ";
            			cin >> mark2;
					} else {
						break;
					} 
				}
            	if (mark1 == 'X' && mark2 == 'O'){
            		Set_color(2);
            		HumanPlayer player1('X');
                	ComputerPlayer player2('O');
                	Game game(&player1, &player2);
                	game.play();
				} else if (mark1 == 'O' && mark2 == 'X'){
					HumanPlayer player1('O');
                	ComputerPlayer player2('X');
                	Game game(&player2, &player1);
                	game.play();
				} 
                break;
            }
            case '2': {
            	Set_color(4);
            	cout << "Human1 vs Human2"<< endl;
            	Set_color(7);
                char mark1, mark2;
            	bool checkMarker1 = true;
            	bool checkMarker2 = true;
				cout << "Player 1(X/O): ";
				cin >> mark1;
            	while (checkMarker1){
            		if(mark1 != 'X' && mark1 != 'O'){
            			cout << "Invalid marker! Please choose marker again player 1: ";
            			cin >> mark1;
					} else {
						break;
					} 
				}
				cout << "Player 2(X/O): ";
				cin >> mark2;
				while (checkMarker2){
            		if(mark2 != 'X' && mark2 != 'O' || mark2 == mark1){
            			cout << "Invalid marker! Please choose marker again player 2: ";
            			cin >> mark2;
					} else {
						break;
					} 
				}
            	if (mark1 == 'X' && mark2 == 'O'){
            		HumanPlayer player1('X');
                	HumanPlayer player2('O');
                	Game game(&player1, &player2);
                	game.play();
				} else if (mark1 == 'O' && mark2 == 'X'){
					HumanPlayer player1('O');
                	HumanPlayer player2('X');
                	Game game(&player2, &player1);
                	game.play();
				} 
                break;
            }
            case '3':{
            	Set_color(4);
            	cout << "Computer1 vs Computer2" << endl;
            	Set_color(7);
            	char mark1, mark2;
            	bool checkMarker1 = true;
            	bool checkMarker2 = true;
				cout << "Player 1(X/O): ";
				cin >> mark1;
            	while (checkMarker1){
            		if(mark1 != 'X' && mark1 != 'O'){
            			cout << "Invalid marker! Please choose marker again player 1: ";
            			cin >> mark1;
					} else {
						break;
					} 
				}
				cout << "Player 2(X/O): ";
				cin >> mark2;
				while (checkMarker2){
            		if(mark2 != 'X' && mark2 != 'O' || mark2 == mark1){
            			cout << "Invalid marker! Please choose marker again player 2: ";
            			cin >> mark2;
					} else {
						break;
					} 
				}
            	if (mark1 == 'X' && mark2 == 'O'){
            		ComputerPlayer player1('X');
                	ComputerPlayer player2('O');
                	Game game(&player1, &player2);
                	game.play();
				} else if (mark1 == 'O' && mark2 == 'X'){
					ComputerPlayer player1('O');
                	ComputerPlayer player2('X');
                	Game game(&player2, &player1);
                	game.play();
				} 
				break;
			}
            case '4':
            	Set_color(2);
                cout << "===============How to play===============" << endl;
                Set_color(3);
            	cout << "||    *Type 1 -> 9 to tick the box     ||" << endl;
            	cout << "||          on the board game          ||" << endl;
            	cout << "||    *The winner is the one who gets  ||" << endl;
            	cout << "||   three X's in a row in Tic Tac Toe ||" << endl;
            	Set_color(2);
            	cout << "=========================================" << endl;
            	cout << endl;
                break;
            case '5':{
            	Set_color(2);
				cout << "===============The game created by===============" << endl;
				Set_color(3);
            	cout << "||               Nhom 6 Studio                 ||" << endl;
            	cout << "||        Contact for more information         ||" << endl;
            	cout << "||          Nhom6@makinggame.hcmue.vn          ||" << endl;
            	Set_color(2);
            	cout << "=================================================" << endl;
            	cout << endl;
            	break;
			}
            case '6':{
            	showOutro0();
    			Sleep(50);
    			showOutro10();
    			Sleep(50);
    			showOutro20();
    			Sleep(50);
    			showOutro30();
    			Sleep(50);
    			showOutro40();
    			Sleep(50);
    			showOutro50();
    			Sleep(50);
    			showOutro60();
    			Sleep(1000);
    			showOutro70();
    			Sleep(50);
    			showOutro80();
    			Sleep(30);
    			showOutro95();
    			Sleep(1000);
    			showOutro100();
    			Sleep(30);
    			clearScreen();
				Set_color(2);
            	cout << "============================================" << endl;
            	Set_color(3);
            	cout << "||        Exited game successfully!       ||" << endl;
                cout << "||    Thank you for enjoying our game^^   ||" << endl;
                Set_color(2);
                cout << "============================================" << endl;
                return 0;
				break;
			}
            default:
            	Set_color(4);
            	cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }

    return 0;
}

