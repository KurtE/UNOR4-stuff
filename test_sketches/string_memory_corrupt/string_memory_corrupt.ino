#include <MemoryHexDump.h>


String patternNames[] = {
  "@@@@@",
  "Light-weight spaceship",
  "R-Pentomino",
  "Diehard",
  ""
};
extern char _Heap_Begin __asm("__HeapBase");   ///< Defined by the linker.
extern char _Heap_Limit __asm("__HeapLimit");  ///< Defined by the linker.
extern "C" caddr_t _sbrk(int incr);

void setup() {
  const char *psz = patternNames[0].c_str();
  char before_serial[5];
  for (uint8_t i = 0; i < 5; i++) before_serial[i] = psz[i];
  Serial.begin(9600);
  delay(1000);

  Serial.print("\nHEAP START: ");
  Serial.println((uint32_t)&_Heap_Begin, HEX);
  Serial.print("HEAP LIMIT: ");
  Serial.println((uint32_t)&_Heap_Limit, HEX);
  Serial.print("SBRK: ");
  Serial.println((uint32_t)_sbrk(0), HEX);

  Serial.println("Looking at the first characters of the first element i the String-Arrazy");
  Serial.println(String(patternNames[0].charAt(0), HEX));
  Serial.println(String(patternNames[0].charAt(1), HEX));
  Serial.println(String(patternNames[0].charAt(2), HEX));
  Serial.println(String(patternNames[0].charAt(3), HEX));
  Serial.println(String(patternNames[0].charAt(4), HEX));

  Serial.println((uint32_t)psz, HEX);
  for (uint8_t i = 0; i < 5; i++) {
    Serial.print(before_serial[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  psz = patternNames[0].after_copy;
  for (uint8_t i = 0; i < 5; i++) {
    Serial.print(*psz++, HEX);
    Serial.print(" ");
  }
  Serial.println();

  MemoryHexDump(Serial, &_Heap_Begin, 16, false);
  extern uint8_t heap_data_save[];
  MemoryHexDump(Serial, heap_data_save, 16, false);



  Serial.end();
}
void loop() {}