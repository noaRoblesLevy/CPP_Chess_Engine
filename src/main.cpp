#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <vector>
#include <chrono>
#include <sstream>

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
bool whiteTurn = true;
int pstPawn[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0},
    { 50, 50, 50, 50, 50, 50, 50, 50},
    { 10, 10, 20, 30, 30, 20, 10, 10},
    {  5,  5, 10, 25, 25, 10,  5,  5},
    {  0,  0,  0, 20, 20,  0,  0,  0},
    {  5, -5,-10,  0,  0,-10, -5,  5},
    {  5, 10, 10,-20,-20, 10, 10,  5},
    {  0,  0,  0,  0,  0,  0,  0,  0}
};
int pstKnight[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};
int pstBishop[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};
int pstRook[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0},
    {  5, 10, 10, 10, 10, 10, 10,  5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    {  0,  0,  0,  5,  5,  0,  0,  0}
};
int pstQueen[8][8] = {
    {-20,-10,-10, -5, -5,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5,  5,  5,  5,  0,-10},
    { -5,  0,  5,  5,  5,  5,  0, -5},
    {  0,  0,  5,  5,  5,  5,  0, -5},
    {-10,  5,  5,  5,  5,  5,  0,-10},
    {-10,  0,  5,  0,  0,  0,  0,-10},
    {-20,-10,-10, -5, -5,-10,-10,-20}
};
int pstKing[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    { 20, 20,  0,  0,  0,  0, 20, 20},
    { 20, 30, 10,  0,  0, 10, 30, 20}
};


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
    std::istringstream ss(input_fen);
    std::string boardPart;
    std::string side;
    std::string castling;
    std::string ep;
    madeMoves.clear();
    ss >> boardPart >> side >> castling >> ep;
    for (int i = 0; i < 8; i++)
    {
        int j = 0;
        for(char c : boardPart){
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
    whiteTurn = (side == "b")?false:true;
    if(castling != ""){
        castleBLong = false;
        castleBShort = false;
        castleWLong = false;
        castleWShort = false;
        for(char c : castling){
            switch (c)
            {
            case 'K':
                castleWShort = true;
                break;
            case 'k':
                castleBShort = true;
                break;
            case 'Q':
                castleWLong = true;
                break;
            case 'q':
                castleBLong = true;
                break;
            default:
                break;
            }
        }
    }else{
        castleBLong = true;
        castleBShort = true;
        castleWLong = true;
        castleWShort = true;
    }
    if(!ep.empty() && ep !="-"){
        Move m;
        int col = ep[0] - 'a';
        m.fromCol = col;
        m.toCol = col;
        if(ep[1] == '3'){
            m.fromRow = 6;
            m.toRow = 4;
        }else{
            m.fromRow = 1;
            m.toRow = 3;
        }
        madeMoves.push_back(m);
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

bool lookForCheck(char board[8][8], int row, int col, bool byWhite){
    //king
    char enemyKing = (byWhite)? 'K' : 'k';
    for (int d = 0; d < 8; d++){
        int r = row + king[d][0];   
        int c = col + king[d][1];   

        if (onBoard(r, c) && board[r][c] == enemyKing)
        {
            return true;
        }
    }
    //knight
    char enemyKnight = (byWhite)? 'N' : 'n';
    for (int d = 0; d < 8; d++){
        int r = row + knight[d][0];   
        int c = col + knight[d][1];   

        if (onBoard(r, c) && board[r][c] == enemyKnight)
        {
            return true;
        }
    }
    //rook + queen
    char enemyRook = (byWhite)? 'R' : 'r';
    char enemyQueen = (byWhite)? 'Q' : 'q';
    for (int d = 0; d < 4; d++){
        int r = row + rook[d][0];
        int c = col + rook[d][1];

        while (onBoard(r, c))
        {
            if ((board[r][c] == enemyRook || board[r][c] == enemyQueen))
            {
                return true;
            }else if (board[r][c] != '.')
            {
                break;
            }
            r += rook[d][0];             
            c += rook[d][1];  
        }
    }
    //bishop + queen
    char enemyBishop = (byWhite)? 'B' : 'b';
    for (int d = 0; d < 4; d++){
        int r = row + bishop[d][0];
        int c = col + bishop[d][1];

        while (onBoard(r, c))
        {
            if ((board[r][c] == enemyBishop || board[r][c] == enemyQueen))
            {
                return true;
            }else if (board[r][c] != '.')
            {
                break;
            }
            r += bishop[d][0];             
            c += bishop[d][1];  
        }
    }
    //pawn
    int enemyPawnDir = (byWhite)? 1 : -1;
    char enemyPawn = (byWhite)? 'P' : 'p';
    int r = row + enemyPawnDir;
    if (onBoard(r, col - 1) && board[r][col-1] == enemyPawn){
        return true;
    }else if(onBoard(r, col + 1) && board[r][col + 1] == enemyPawn){
        return true;
    }
    return false;
}

void castleCheck(char board[8][8], int row, int col){
    char p = board[row][col];
    if(std::isupper(p) && p == board[7][4]){
        if(castleWShort){
            if (board[row][col+1] == '.' && board[row][col+2] == '.' && board[row][7] == 'R')
            {
                if(!lookForCheck(board, row, col, false) && !lookForCheck(board, row, col+1, false)){
                    fillMove(row, col, row, col+2);
                    possibleMoves.back().isCastle = true;
                }
            }   
        }
    }
    if(std::isupper(p) && p == board[7][4]){
        if(castleWLong){
            if (board[row][col-1] == '.' && board[row][col-2] == '.' && board[row][col-3] == '.' && board[row][0] == 'R')
            {
                if(!lookForCheck(board, row, col, false) && !lookForCheck(board, row, col-1, false)){
                    fillMove(row, col, row, col-2);
                    possibleMoves.back().isCastle = true;
                }
            }   
        }
    }

    if(std::islower(p) && p == board[0][4]){
        if(castleBShort){
            if (board[row][col+1] == '.' && board[row][col+2] == '.' && board[row][7] == 'r')
            {
                if(!lookForCheck(board, row, col, true) && !lookForCheck(board, row, col+1, true)){
                    fillMove(row, col, row, col+2);
                    possibleMoves.back().isCastle = true;
                }
            }   
        }
    }
    if(std::islower(p) && p == board[0][4]){
        if(castleBLong){
            if (board[row][col-1] == '.' && board[row][col-2] == '.' && board[row][col-3] == '.' && board[row][0] == 'r')
            {
                if(!lookForCheck(board, row, col, true) && !lookForCheck(board, row, col-1, true)){
                    fillMove(row, col, row, col-2);
                    possibleMoves.back().isCastle = true;
                }
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

int findKing(char board[8][8], bool White){
    char t = (White)? 'K' : 'k';
    for(int i = 0; i<8; i++){
        for (int j = 0; j < 8; j++){
            if(board[i][j] == t){
                return i  * 8 + j;
            }
        }
    }
    return -1;
}

void filterLegalMoves(char board[8][8]){
    std::vector<Move> legalMoves;
    for(Move m : possibleMoves){
        bool White = (std::isupper(board[m.fromRow][m.fromCol]));
        makeMove(board, m);
        int coord = findKing(board, White);
        int row = coord / 8;
        int col = coord % 8;
        
        if(!lookForCheck(board, row, col, !White)){
            legalMoves.push_back(m);
        }
        unmakeMove(board);
        
    }
    possibleMoves = legalMoves;
}

void moveListPerColor(char board[8][8], bool white){
    possibleMoves.clear();
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++)
        {
            if(white)
            {
                if (std::isupper(board[i][j])){
                    findMoves(board, i, j);
                }
            }else{
                if (std::islower(board[i][j])){
                    findMoves(board, i, j);
                }
            }
            
        }       
    }
    filterLegalMoves(board);
}

long perft(char board[8][8], int depth, bool white){
    if(depth == 0){return 1;}
    long count = 0;
    moveListPerColor(board, white);
    std::vector<Move> initMoves = possibleMoves;
    for(Move m : initMoves){
        makeMove(board, m);
        count += perft(board, depth -1, !white);
        unmakeMove(board);
    }
    return count;
}

int assignPoints(char piece, bool isWhite, int i, int j){
    int sign = (isWhite)? 1 : -1;
    int value = 0;
    switch (std::toupper(piece))
    {
    case 'K':
        value = 0;
        value += (isWhite) ? pstKing[i][j] : pstKing[7 - i][j];
        break;
    case 'Q':
        value = 900;
        value += (isWhite) ? pstQueen[i][j] : pstQueen[7 - i][j];
        break;
    case 'R':
        value = 500;
        value += (isWhite) ? pstRook[i][j] : pstRook[7 - i][j];
        break;
    case 'B':
        value = 330;
        value += (isWhite) ? pstBishop[i][j] : pstBishop[7 - i][j];
        break;
    case 'N':
        value = 320;
        value += (isWhite) ? pstKnight[i][j] : pstKnight[7 - i][j];
        break;
    case 'P':
        value = 100;
        value += (isWhite) ? pstPawn[i][j] : pstPawn[7 - i][j];
        break;
    default:
        break;
    }
    value *= sign;
    return value;
}

int evaluate(char board[8][8]){
    int count = 0;
    for(int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++)
        {
            char piece = board[i][j];
            count += assignPoints(piece, std::isupper(piece), i, j);
        }
        
    }
    return count;
}

bool parseMove(char board[8][8], std::string text, bool white, Move& result){
    
    int fromCol = text[0] - 'a';
    int fromRow = 8 - (text[1] - '0');
    int toCol = text[2] - 'a';
    int toRow = 8 - (text[3] - '0');
    char promPiece = (text.size()==5)?text[4]:'.';

    moveListPerColor(board, white);
    for(Move m : possibleMoves){
        if(m.fromCol == fromCol && m.fromRow == fromRow && m.toCol == toCol && m.toRow == toRow && (std::tolower(m.promotionPiece) == promPiece)){
            result = m;
            return true;
        }
    }

    return false;
}

std::string moveToString(Move m){
    char fromRow = '0' +  (8 - m.fromRow);
    char fromCol = 'a' + m.fromCol;
    char toRow = '0' +  (8 - m.toRow);
    char toCol = 'a' + m.toCol;
    std::string move;
    move += fromCol;
    move += fromRow;
    move += toCol;
    move += toRow;
    if(m.promotionPiece != '.'){
        move += std::tolower(m.promotionPiece);
    }
    return move;
}

void bench(char board[8][8]){
    auto start = std::chrono::steady_clock::now();
    long long count = perft(board, 5, true);
    auto end = std::chrono::steady_clock::now();
    double seconds = std::chrono::duration<double>(end - start).count();
    std::cout << count << " in " << seconds << " s, " << count / seconds << " nps\n";

}

int minmax(char board[8][8], int depth, bool white){
    if(depth == 0){return evaluate(board);}
    moveListPerColor(board, white);
    std::vector<Move> minmaxMoves = possibleMoves;
    int kingPos = findKing(board, white);
    if(minmaxMoves.empty() && lookForCheck(board, kingPos / 8, kingPos % 8, !white)){
        int score = (white)? -100000 : 100000;
        return score;
    }else if(minmaxMoves.empty()){
        return 0;
    }
    int bestScore = white ? -1000000 : 1000000;
    for(Move m: minmaxMoves){
        makeMove(board, m);
        int score = minmax(board, depth - 1, !white);
        unmakeMove(board);
        if (white &&  score> bestScore) {
            bestScore = score;
        }else if (!white &&  score < bestScore) {
            bestScore = score;
        }
    }
    return bestScore;
}

void uci(){
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream ss(line);
        std::string command;
        ss >> command;

        if(command == "uci"){
            std::cout << "id name NoaChessEngine" << '\n';
            std::cout << "id author Noa" << '\n';
            std::cout << "uciok" << std::endl;
        }else if(command == "isready"){
            std::cout << "readyok" << std::endl;
        }else if(command == "position"){
            std::string pos;
            ss >> pos;
            if(pos == "startpos"){
                loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
            }else if(pos == "fen"){
                std::string fen;
                std::string temp;
                for(int i = 0; i < 6; i++){
                    ss >> temp;
                    fen+=temp; 
                    fen+=" ";
                }
                loadFEN(fen);
            }
            std::string moves;
            ss >> moves;
            while(ss >> moves){
                Move m;
                if(parseMove(new_board, moves, whiteTurn, m)){
                    makeMove(new_board, m);
                    whiteTurn = !whiteTurn;
                }
            }
        }else if(command == "go"){
            moveListPerColor(new_board, whiteTurn);
            std::vector<Move> goMoves = possibleMoves;
            int bestScore = whiteTurn ? -1000000 : 1000000;
            if(goMoves.empty()){
                std::cout << "bestmove 0000" << std::endl;
            }else{
                Move bestMove = goMoves[0];
                for(Move m : goMoves){
                    makeMove(new_board, m);
                    int score = minmax(new_board, 3, !whiteTurn);
                    if (whiteTurn &&  score> bestScore) {
                        bestScore = score;
                        bestMove = m;
                    }else if (!whiteTurn &&  score < bestScore) {
                        bestScore = score;
                        bestMove = m;
                    }
                    unmakeMove(new_board);
                }
                std::string move = moveToString(bestMove);
                std::cout << "bestmove " << move << std::endl;
            }

        }else if(command == "d"){
            showBoard(new_board);
        }else if(command == "quit"){
            return;
        }else if(command == "eval"){
            int eval = evaluate(new_board);
            std::cout << eval << std::endl;
        }
    }
}


int main() {
    std::cout << "Chess engine v0.1\n";
    // loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    // bench(new_board);

    uci();
    return 0;
}
