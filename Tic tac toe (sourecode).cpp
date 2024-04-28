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
    	cout << "-------------" << endl;
    	for (int i = 0; i < 3; ++i) {
        	cout << "| ";
        	for (int j = 0; j < 3; ++j) {
            	cout << arr[i][j] << " | ";
        	}
        	cout << endl;
        	cout << "-------------" << endl;
    	}
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
        int row, col;
        cout << "Enter your move (row and column, (x <= 3, y <= 3)): ";
        cin >> row >> col;
        if (row < 1 || row > 3 || col < 1 || col > 3 || !board.placeMarker(row - 1, col - 1, marker)) {
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
            cout << "Tic Tac Toe 3x3 Gameplay" << endl;
            board.printBoard();
            cout << endl;

            try {
                current->makeMove(board);
            } catch (const invalid_argument& e) {
                cerr << "Error: " << e.what() << endl;
                continue;
            }

            char winner = board.checkWinner();
            if (winner != '-') {
                cout << "Winner is: " << winner << endl;
                board.printBoard();
                break;
            } else if (board.full()) {
                cout << "TIE!" << endl;
                board.printBoard(); 
                break;
            }

            switchPlayers(current);
        }
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

// Xay dung giao dien menu cua game
void Menu() {
	Set_background_color(7);
    cout << "===========[X] Tic Tac Toe Game [O]============" << endl;
    cout << "||         1. Play against Computer          ||" << endl;
    cout << "||          2. Play with a Friend            ||" << endl;
    cout << "||          3. Computer vs Computer          ||" << endl;
    cout << "||             4. How to play                ||" << endl;
    cout << "||             5. Information                ||" << endl;
    cout << "||              6. Exit Game                 ||" << endl;
    cout << "===============================================" << endl;
}

int main() {
    srand(time(NULL));
    int choice; // tao ra bien de lua chon cac muc trong menu gameplay
    while (1) {
    	Menu();
    	
    	Set_color(2);
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
            	Set_background_color(7);
            	cout << "Human vs Computer"<< endl;
				Set_color(3);
                HumanPlayer player1('X');
                ComputerPlayer player2('O');
                Game game(&player1, &player2);
                game.play();
                break;
            }
            case 2: {
            	Set_background_color(7);
            	cout << "Human1 vs Human2"<< endl;
				Set_color(5);
                HumanPlayer player1('X');
                HumanPlayer player2('O');
                Game game(&player1, &player2);
                game.play();
                break;
            }
            case 3:{
            	Set_background_color(7);
            	cout << "Computer1 vs Computer2" << endl;
            	Set_color(4);
            	ComputerPlayer player1('X');
            	ComputerPlayer player2('O');
            	Game game(&player1, &player2);
            	game.play();
				break;
			}
            case 4:
            	Set_background_color(7);
                cout << "===============How to play===============" << endl;
            	cout << "||    *Type (x(row), y(col)) to tick   ||" << endl;
            	cout << "||      the box on the board game      ||" << endl;
            	cout << "||    *The winner is the one who gets  ||" << endl;
            	cout << "||   three X's in a row in Tic Tac Toe ||" << endl;
            	cout << "=========================================" << endl;
            	cout << endl;
                break;
            case 5:{
            	Set_background_color(7);
				cout << "===============The game created by===============" << endl;
            	cout << "||               Nhom 6 Studio                 ||" << endl;
            	cout << "||        Contact for more information         ||" << endl;
            	cout << "=================================================" << endl;
            	cout << endl;
            	break;
			}
            case 6:{
            	Set_background_color(7);
            	cout << "============================================" << endl;
            	cout << "||             Exiting game...            ||" << endl;
                cout << "||    Thank you for enjoying our game!!   ||" << endl;
                cout << "============================================" << endl;
                return 0;
				break;
			}
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }

    return 0;
}


