int mid(int x, int y, int z) {
  int m;
  m = z;                 // 1
  if (y < z) {           // 2
    if (x < y) {         // 3
      m = y;             // 4
    } else if (x < z) {  // 5
      m = x;             // 6
    }
  } else {               // 7
    if (x > y) {         // 8
      m = y;             // 9
    } else if (x > z) {  // 10
      m = x;             // 11
    }
  }
  return m;  // 12
}