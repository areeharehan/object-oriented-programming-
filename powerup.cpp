#include <iostream>
#include <string>
using namespace std;

#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BOLD        "\033[1m"

#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"
#define BG_MAGENTA  "\033[45m"
#define BG_CYAN     "\033[46m"

class Player {
protected:
    string name;
    int score;
public:
    Player(string n) {
        name = n;
        score = 0;
    }
    string getName()  const {
        return name;
    }
    int    getScore() const {
        return score;
    }
    void addPoint(int pts = 1) {
        score += pts;
    }
    void resetScore() {
        score = 0;
    }
    virtual char makeChoice() = 0;
    virtual ~Player() {}
};

class HumanPlayer : public Player {
public:
    HumanPlayer(string n) : Player(n) {}
    char makeChoice() {
        char move;
        while (true) {
            cout << CYAN << "  Your move (R/P/S): " << RESET;
            cin >> move;
            move = toupper(move);
            if (move == 'R' || move == 'P' || move == 'S') return move;
            cout << RED << "  Invalid! Please enter R, P, or S.\n" << RESET;
        }
    }
};

class ComputerPlayer : public Player {
public:
    ComputerPlayer(string n) : Player(n) {}
    virtual char makeChoice() = 0;
    virtual ~ComputerPlayer() {}
};

class EasyComputer : public ComputerPlayer {
public:
    EasyComputer() : ComputerPlayer("Easy Computer") {
    }
    char makeChoice() { return 'R'; }
};

class MediumComputer : public ComputerPlayer {
    int counter;
public:
    MediumComputer() : ComputerPlayer("Medium Computer"), counter(0) {}
    char makeChoice() {
        counter++;
        if (counter % 3 == 1) return 'R';
        if (counter % 3 == 2) return 'P';
        return 'S';
    }
};

class HardComputer : public ComputerPlayer {
    char lastPlayerMove;
public:
    HardComputer() : ComputerPlayer("Hard Computer"), lastPlayerMove(' ') {}
    void setLastPlayerMove(char move) { lastPlayerMove = move; }
    char makeChoice() {
        if (lastPlayerMove == ' ') return 'R';
        if (lastPlayerMove == 'R') return 'P';
        if (lastPlayerMove == 'P') return 'S';
        return 'R';
    }
};

string moveName(char c) {
    if (c == 'R') return "Rock";
    if (c == 'P') return "Paper";
    return "Scissors";
}

int roundResult(char human, char comp) {
    if (human == comp) return 0;
    if ((human == 'R' && comp == 'S') ||
        (human == 'P' && comp == 'R') ||
        (human == 'S' && comp == 'P'))
        return 1;
    return -1;
}

class Game {
private:
    HumanPlayer* human;
    ComputerPlayer* computer;

    int  powerUp;
    bool powerUpActive;

    void displayPowerUpStatus() {
        if (powerUp > 0)
            cout << MAGENTA << BOLD << "  [POWER-UP] Double Down: AVAILABLE (x1)\n" << RESET;
        else
            cout << RED << BOLD << "  [POWER-UP] Double Down: CONSUMED\n" << RESET;
    }

    void askForPowerUp() {
        powerUpActive = false;

        if (powerUp <= 0) {
            cout << RED << "  Power-up already consumed this match!\n" << RESET;
            return;
        }

        char choice;
        cout << YELLOW << "  Use Double Down power-up? Win = 2 pts (Y/N): " << RESET;
        cin >> choice;
        choice = toupper(choice);

        if (choice == 'Y') {
            powerUp--;
            powerUpActive = true;
            cout << GREEN << BOLD << "  Power-up activated! A win this round = 2 points!\n" << RESET;
        }
        else {
            cout << CYAN << "  Power-up saved.\n" << RESET;
        }
    }

public:
    Game(HumanPlayer* h, ComputerPlayer* c) {
        human = h;
        computer = c;
        powerUp = 1;
    }

    bool playMatch() {
        human->resetScore();
        computer->resetScore();
        powerUp = 1;

        cout << "\n";
        cout << BG_BLUE << BOLD << WHITE << "====================================================" << RESET << "\n";
        cout << BG_BLUE << BOLD << WHITE << "  Level: " << computer->getName() << RESET << "\n";
        cout << BG_BLUE << BOLD << WHITE << "  You have 1x Double Down power-up!" << RESET << "\n";
        cout << BG_BLUE << BOLD << WHITE << "====================================================" << RESET << "\n";

        int roundsPlayed = 0;

        while (human->getScore() < 2 && computer->getScore() < 2 && roundsPlayed < 3) {
            roundsPlayed++;
            cout << "\n" << BOLD << YELLOW << "  Round " << roundsPlayed << " of 3\n" << RESET;
            cout << YELLOW << "----------------------------------------------------\n" << RESET;

            displayPowerUpStatus();
            askForPowerUp();

            char hMove = human->makeChoice();

            HardComputer* hard = dynamic_cast<HardComputer*>(computer);
            if (hard != nullptr) hard->setLastPlayerMove(hMove);

            char cMove = computer->makeChoice();

            cout << CYAN << "  " << human->getName() << " chose: " << moveName(hMove) << RESET << "\n";
            cout << MAGENTA << "  " << computer->getName() << " chose: " << moveName(cMove) << RESET << "\n";

            int result = roundResult(hMove, cMove);

            if (result == 1) {
                int r = powerUpActive ? 2 : 1;
                human->addPoint(r);

                if (powerUpActive)
                    cout << BG_GREEN << BOLD << WHITE << "  DOUBLE WIN! You get 2 points!" << RESET << "\n";
                else
                    cout << BG_GREEN << BOLD << WHITE << "  YOU WIN THIS ROUND!" << RESET << "\n";
            }
            else if (result == -1) {
                computer->addPoint();
                cout << BG_RED << BOLD << WHITE << "  Computer wins this round!" << RESET << "\n";
            }
            else {
                cout << BG_YELLOW << BOLD << WHITE << "  It's a TIE!" << RESET << "\n";
            }

            cout << BOLD << "  Score -> You: " << GREEN << human->getScore()
                << RESET << BOLD << "  |  " << computer->getName() << ": "
                << RED << computer->getScore() << RESET << "\n";
            cout << YELLOW << "----------------------------------------------------\n" << RESET;
        }

        cout << "\n";
        cout << BG_CYAN << BOLD << WHITE << "====================================================" << RESET << "\n";
        if (human->getScore() > computer->getScore()) {
            cout << BG_GREEN << BOLD << WHITE << "  YOU WIN THE MATCH "
                << human->getScore() << "-" << computer->getScore() << "!" << RESET << "\n";
            cout << BG_CYAN << BOLD << WHITE << "====================================================" << RESET << "\n";
            return true;
        }
        else if (computer->getScore() > human->getScore()) {
            cout << BG_RED << BOLD << WHITE << "  COMPUTER WINS THE MATCH "
                << computer->getScore() << "-" << human->getScore() << "!" << RESET << "\n";
            cout << BG_CYAN << BOLD << WHITE << "====================================================" << RESET << "\n";
            return false;
        }
        else {
            cout << BG_YELLOW << BOLD << WHITE << "  MATCH DRAWN "
                << human->getScore() << "-" << computer->getScore() << "!" << RESET << "\n";
            cout << BG_CYAN << BOLD << WHITE << "====================================================" << RESET << "\n";
            return false;
        }
    }
};

int main() {
    cout << "\n";
    cout << BG_MAGENTA << BOLD << WHITE << "  ==========================================" << RESET << "\n";
    cout << BG_MAGENTA << BOLD << WHITE << "   ROCK PAPER SCISSORS - 3 LEVEL CHALLENGE " << RESET << "\n";
    cout << BG_MAGENTA << BOLD << WHITE << "  ==========================================" << RESET << "\n";

    cout << "\n" << CYAN << "  Enter your name: " << RESET;
    string playerName;
    cin >> playerName;

    HumanPlayer human(playerName);
    char playAgain = 'Y';

    while (playAgain == 'Y' || playAgain == 'y') {
        cout << "\n" << BOLD << YELLOW << "  Choose Computer Level:\n" << RESET;
        cout << GREEN << "  1. Easy   (Always Rock)\n" << RESET;
        cout << YELLOW << "  2. Medium (Cycles R/P/S)\n" << RESET;
        cout << RED << "  3. Hard   (Beats your last move)\n" << RESET;
        cout << CYAN << "  Enter your choice: " << RESET;

        int choice;
        cin >> choice;

        ComputerPlayer* computer = nullptr;
        if (choice == 1) computer = new EasyComputer();
        else if (choice == 2) computer = new MediumComputer();
        else if (choice == 3) computer = new HardComputer();
        else {
            cout << RED << "  Invalid choice! Defaulting to Easy.\n" << RESET;
            computer = new EasyComputer();
        }

        Game game(&human, computer);
        game.playMatch();
        delete computer;

        cout << CYAN << "\n  Play again? (Y/N): " << RESET;
        cin >> playAgain;
    }

    cout << "\n" << BG_MAGENTA << BOLD << WHITE << "  Thanks for playing! Goodbye." << RESET << "\n";
    return 0;
}