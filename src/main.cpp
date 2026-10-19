#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <vector>

char start_board[8][8] = {
        {'r','n','b','q','k','b','n','r'},  // rij 8: zwarte stukken
        {'p','p','p','p','p','p','p','p'},  // rij 7: zwarte pionnen
        {'.','.','.','.','.','.','.','.'},  // rij 6
        {'.','.','.','.','.','.','.','.'},  // rij 5
        {'.','.','.','.','.','.','.','.'},  // rij 4
        {'.','.','.','.','.','.','.','.'},  // rij 3
        {'P','P','P','P','P','P','P','P'},  // rij 2: witte pionnen
        {'R','N','B','Q','K','B','N','R'}  // rij 1: witte stukken
    };
char new_board[8][8];
int knight[8][2] = {
    {-2,-1}, {-2, 1}, { 2,-1}, { 2, 1},
    { 1,-2}, {-1,-2}, {-1, 2}, { 1, 2}
};
int king[8][2] = {
    {-1,-1}, {-1, 1}, { 1,-1}, { 1, 1},
    { 1,0}, {-1,0}, {0, 1}, { 0, -1}
};
int rook[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
};
int queen[8][2] = {
        {-1,-1}, {-1, 1}, { 1,-1}, { 1, 1},
        { 1,0}, {-1,0}, {0, 1}, { 0, -1}
};
int bishop[4][2] = {
        {-1, -1}, {1, 1}, {1, -1}, {-1, 1}
};
int pawn[2][2] = {
        {1, 0},{2, 0}
};
struct Move {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
    char takenPiece = '.';
    char promotionPiece = '.';
    bool enPassant = false;
    bool prevBLong = true;
    bool prevBShort = true;
    bool prevWLong = true;
    bool prevWShort = true;
    bool isCastle = false;
};
std::vector<Move> possibleMoves; 
std::vector<Move> madeMoves;
bool castleBLong= true;
bool castleBShort= true;
bool castleWLong= true;
bool castleWShort= true;

void showBoard(char board[8][8]){
    for (int i = 0; i < 8; i++)
    {
        std::cout << 8 - i<< " ";
        for (int j = 0; j < 8; j++)
        {
            std::cout << board[i][j] << " ";
        }
        std::cout << "\n";       
    }
    std::cout << "  a b c d e f g h" << "\n"; 
    
}

void showFEN(char board[8][8]){
    std::string FEN = "";
    std::stack<char> fen_points;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            //punten omzetten naar een getal
            if(board[i][j] == '.'){
                fen_points.push(board[i][j]);
            }
            else{
                if (fen_points.size()!=0)
                {
                    FEN += std::to_string(fen_points.size());
                    while (fen_points.size()>0)
                    {
                        fen_points.pop();
                    }
                }
                FEN += board[i][j];
            }
        }
        if (fen_points.size() != 0)     // nu pas: één keer per rij
        {
            FEN += std::to_string(fen_points.size());
            while (fen_points.size() > 0)
            {
                fen_points.pop();
            }
        }
        if (i<7)
        {
            FEN+="/";
        }
    }
    std::cout << FEN << '\n';
}

void loadFEN(std::string input_fen){
    for (int i = 0; i < 8; i++)
    {
        int j = 0;
        for(char c : input_fen){
            if(c != '/'){
                if (c >= '1' && c <= '8')
                {
                    for (int k = 0; k < c - '0'; k++)
                    {
                        new_board[i][j] = '.';
                        j++;
                    }
                    
                    
                }else{
                    new_board[i][j] = c;
                    j++;
                }
            }            
            else
            {
                i++;
                j = 0;
            }
            
        }
    }

}

void fillMove(int fromRow, int fromCol, int toRow, int toCol){
    Move m;
    m.fromCol = fromCol;
    m.fromRow = fromRow;
    m.toCol = toCol;
    m.toRow = toRow;
    possibleMoves.push_back(m);
}

void showMoveList(){
    for(Move m : possibleMoves){
        char fCol = 'a' + m.fromCol;
        char tCol = 'a' + m.toCol;
        std::cout << fCol << 8 - m.fromRow << ',';
        std::cout << tCol << 8 - m.toRow;
        if (m.promotionPiece != '.')
        {
            std::cout << m.promotionPiece;
        }if (m.takenPiece != '.'){
            std::cout << 'x' << m.takenPiece;
        }
        std::cout << '\n';
    }
}

void promotePawn(int row, int col, int r, int c, char piece){
    char pp[4] = {'r', 'n', 'b', 'q'};
    for (int i = 0; i < 4; i++){
        fillMove(row, col, r, c);
        possibleMoves.back().promotionPiece = (std::isupper(piece)) ? toupper(pp[i]) : pp[i];                 
    }
}

bool onBoard(int row, int col){
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool isEnemy(char a, char b){
    return std::isupper(a) != std::isupper(b);
}

void castleCheck(char board[8][8], int row, int col){
    char p = board[row][col];
    if(std::isupper(p) && p == board[7][4]){
        if(castleWShort){
            if (board[row][col+1] == '.' && board[row][col+2] == '.' && board[row][7] == 'R')
            {
                fillMove(row, col, row, col+2);
                possibleMoves.back().isCastle = true;
            }   
        }
    }
    if(std::isupper(p) && p == board[7][4]){
        if(castleWLong){
            if (board[row][col-1] == '.' && board[row][col-2] == '.' && board[row][col-3] == '.' && board[row][0] == 'R')
            {
                fillMove(row, col, row, col-2);
                possibleMoves.back().isCastle = true;
            }   
        }
    }

    if(std::islower(p) && p == board[0][4]){
        if(castleBShort){
            if (board[row][col+1] == '.' && board[row][col+2] == '.' && board[row][7] == 'r')
            {
                fillMove(row, col, row, col+2);
                possibleMoves.back().isCastle = true;
            }   
        }
    }
    if(std::islower(p) && p == board[0][4]){
        if(castleBLong){
            if (board[row][col-1] == '.' && board[row][col-2] == '.' && board[row][col-3] == '.' && board[row][0] == 'r')
            {
                fillMove(row, col, row, col-2);
                possibleMoves.back().isCastle = true;
            }   
        }
    }
}

void slideMoves(char board[8][8], int row, int col, int dirs[][2], int count){
    char piece = board[row][col];
    for (int d = 0; d < count; d++){
        int r = row + dirs[d][0];
        int c = col + dirs[d][1];

        while (onBoard(r, c))
        {
            if (board[r][c] == '.')
            {
                fillMove(row, col, r, c);
            }else if (isEnemy(piece, board[r][c]))
            {
                fillMove(row, col, r, c);
                break;
            }else{break;}
                r += dirs[d][0];             
                c += dirs[d][1];  
        }
    }
}

void stepMoves(char board[8][8], int row, int col, int dirs[][2], int count){
    char piece = board[row][col];
    for (int d = 0; d < count; d++){
        int r = row + dirs[d][0];   
        int c = col + dirs[d][1];   

        if (onBoard(r, c))
        {
            if (board[r][c] == '.')
            {
                fillMove(row, col, r, c);
            }else if (isEnemy(piece, board[r][c]))
            {
                fillMove(row, col, r, c);
            }           
        }
    }
    if(std::toupper(piece) == 'K'){
        castleCheck(board, row, col);
    }  
}

void pawnMoves(char board[8][8], int row, int col){
    char piece = board[row][col];
    int dir      = std::isupper(piece) ? -1 : 1;
    int startRow = std::isupper(piece) ?  6 : 1;
    int r = row + dir;
    //take
    if (onBoard(r, col-1) && board[r][col-1] != '.' && isEnemy(piece, board[r][col-1]))
    {
        if(r == 0 || r == 7){
            promotePawn(row, col, r, col -1, piece);
        }else{
            fillMove(row, col, r, col - 1);
        }
    }
    if (onBoard(r, col+1) && board[r][col+1] != '.' && isEnemy(piece, board[r][col+1]))
    {
        if(r == 0 || r == 7){
            promotePawn(row, col, r, col +1, piece);
        }else{
            fillMove(row, col, r, col + 1);
        }
    }
    // en passant
    if (!madeMoves.empty()){
        Move m = madeMoves.back();
        if (onBoard(r, col+1) && isEnemy(piece, board[row][col+1]) && (board[row][col+1] == 'P' || board[row][col+1] == 'p')){
            if(abs(m.toRow - m.fromRow) == 2 && m.toCol == col + 1 && m.toRow == row){
                fillMove(row, col, r, col + 1);
                possibleMoves.back().enPassant = true;
            }
        }
        if (onBoard(r, col-1) && isEnemy(piece, board[row][col-1]) && (board[row][col-1] == 'P' || board[row][col-1] == 'p')){
            if(abs(m.toRow - m.fromRow) == 2 && m.toCol == col - 1 && m.toRow == row){
                fillMove(row, col, r, col - 1);
                possibleMoves.back().enPassant = true;
            }
        }
    }

    //promotion
    if((r == 0 || r == 7) && board[r][col] == '.'){
        promotePawn(row, col, r, col, piece);
    }
    else{
        if(onBoard(r, col) && board[r][col] == '.'){
        fillMove(row, col, r, col);
        if (onBoard(r+dir, col) && board[r+dir][col] == '.' && row == startRow){
            fillMove(row, col, r + dir, col);
        }
    }
    }
    
}

void findMoves(char board[8][8], int row, int col){

    char piece = board[row][col];
    switch (piece){
    case 'N':
    case 'n':
        stepMoves(board, row, col, knight, 8);
        break;
    case 'K':
    case 'k':
        stepMoves(board, row, col, king, 8);
        break;
    case 'R':
    case 'r':
        slideMoves(board, row, col, rook, 4);
        break;
    case 'Q':
    case 'q':
        slideMoves(board, row, col, queen, 8);
        break;
    case 'B':
    case 'b':
        slideMoves(board, row, col, bishop, 4);
        break;
    case 'P':
    case 'p':
        pawnMoves(board, row, col);        
        break;
        
    default:
        break;
    }
}

void makeMove(char board[8][8], Move m){
    char p = board[m.fromRow][m.fromCol];
    if(isEnemy(p, board[m.toRow][m.toCol])){
        m.takenPiece = board[m.toRow][m.toCol];
    }else{
        m.takenPiece = '.';
    }

    if(m.enPassant){
        m.takenPiece = board[m.fromRow][m.toCol];
    }

    if(m.promotionPiece != '.'){
        board[m.toRow][m.toCol] = m.promotionPiece;
    }else{board[m.toRow][m.toCol] = p;}

    m.prevBLong = castleBLong;
    m.prevBShort = castleBShort;
    m.prevWLong = castleWLong;
    m.prevWShort = castleWShort;

    if(m.isCastle){
        if ((m.fromCol - m.toCol) < 0)
        {
            board[m.toRow][m.toCol -1] = board[m.toRow][7];
            board[m.toRow][7] = '.';
        }else{
            board[m.toRow][m.toCol +1] = board[m.toRow][0];
            board[m.toRow][0] = '.';
        }
        
    }

    madeMoves.push_back(m);
    board[m.fromRow][m.fromCol] = '.';
    if(m.enPassant){board[m.fromRow][m.toCol] = '.';}
    


    if (std::toupper(p) == 'K')
    {
        if (std::isupper(p))
        {
            castleWLong = false;
            castleWShort = false;
        }else{
            castleBLong = false;
            castleBShort = false;
        }
           
    }
    
    //{0,0}, {0,7}, {7,0}, {7,7}
    if((m.fromRow == 0 && m.fromCol == 0) || (m.toRow == 0 && m.toCol == 0)){
        castleBLong = false;
    }if((m.fromRow == 0 && m.fromCol == 7) || (m.toRow == 0 && m.toCol == 7)){
        castleBShort = false;
    }if((m.fromRow == 7 && m.fromCol == 0) || (m.toRow == 7 && m.toCol == 0)){
        castleWLong = false;
    }if((m.fromRow == 7 && m.fromCol == 7) || (m.toRow == 7 && m.toCol == 7)){
        castleWShort = false;
    }
}

void unmakeMove(char board[8][8]){
    Move m = madeMoves.back();
    char p;
    if (m.promotionPiece != '.'){
        std::isupper(m.promotionPiece) ? p = 'P' : p = 'p';
    }else{p = board[m.toRow][m.toCol];}    
    board[m.fromRow][m.fromCol] = p;
    if(m.enPassant){
        board[m.fromRow][m.toCol] = m.takenPiece;
        board[m.toRow][m.toCol] = '.';
    }else{board[m.toRow][m.toCol] = m.takenPiece;}

    castleBLong = m.prevBLong;
    castleBShort = m.prevBShort;
    castleWLong = m.prevWLong;
    castleWShort = m.prevWShort;
    if (m.isCastle)
    {
        if ((m.fromCol - m.toCol) < 0)
        {
            board[m.fromRow][7] = board[m.fromRow][m.toCol-1];
            board[m.fromRow][m.toCol-1] = '.';
        }else{
            board[m.fromRow][0] = board[m.fromRow][m.toCol+1];
            board[m.fromRow][m.toCol+1] = '.';
        }        
    }
    

    madeMoves.pop_back();
}

int main() {
    std::cout << "Chess engine v0.1\n";
    loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPP1PPP/RNBQKBNR");
    showBoard(new_board);
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (std::isupper(new_board[i][j]))
            {
                findMoves(new_board, i, j);
            }
        }       
    }
    showMoveList();
    std::cout << possibleMoves.size();

    makeMove(new_board, possibleMoves[20]);
    unmakeMove(new_board);
    return 0;
}
