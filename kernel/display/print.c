#include "print.h"

//下一个可用位置
Byte Row = 0;
Byte Col = 0;
Byte RowCount = 25;
Byte ColCount = 80;

void Write(Byte row, Byte col, Char c) {
    Word *memAddr = (Word *)(QuadWord)0xb8000; //字符模式输出地址
    memAddr += row * ColCount + col;
    Word data = 0x0f00;
    data |= c;
    *memAddr = data;
}

Char Read(Byte row, Byte col) {
    Word *memAddr = (Word *)(QuadWord)0xb8000; //字符模式输出地址
    memAddr += row * ColCount + col;
    return (Char)*memAddr;
}

void Scroll(Byte count) {
    if (count == 0) {
        return;
    }
    if (count >= ColCount - 1) {
        count = ColCount - 1;
    }
    Byte oldRow = 0;
    for (Byte newRow = count; newRow < ColCount; ++oldRow, ++newRow) {
        for (Byte col = 0; col < ColCount; ++col) {
            Write(oldRow, col, Read(newRow, col));
        }
    }
    //清屏
    for (; oldRow < ColCount; ++oldRow) {
        for (Byte col = 0; col < ColCount; ++col) {
            Write(oldRow, col, ' ');
        }
    }
    Row -= count;
}

void Print(char *str) {
    while (*str != 0) {
        Char c = *str;
        switch (c) {
        case '\t': {
            Byte maxCol = ((Col - 1) / 4 + 1) * 4;
            if (maxCol > ColCount) {
                maxCol = ColCount;
            }
            for (; Col < maxCol; ++Col) {
                Write(Row, Col, ' ');
            }
            break;
        }
        case '\n': {
            for (; Col <= (ColCount - 1); ++Col) {
                Write(Row, Col, ' ');
            }
            break;
        }
        default: {
            Write(Row, Col, c);
            ++Col;
        }
        }
        if (Col >= ColCount) {
            ++Row;
            Col = 0;
        }
        if (Row >= RowCount) {
            //向上卷屏
            Scroll(1);
        }
        ++str;
    }
}

void ClearScreen() {
    for (Byte row = 0; row < RowCount; ++row) {
        for (Byte col = 0; col < ColCount; ++col) {
            Write(row, col, ' ');
        }
    }
}
