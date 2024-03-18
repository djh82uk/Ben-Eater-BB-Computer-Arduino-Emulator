#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

uint8_t frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

int clock_delay_value = 1;
int a_x_axis = 0;
int b_x_axis = 1;
int inst_x_axis = 2;
int out_x_axis = 3;
int flag_x_axis = 4;
int memreg_x_axis = 4;
bool flag_reg_input_enabled = false;
int prog_counter = 0;
bool counter_enabled = true;
byte a_reg = 0b00000000;
byte b_reg = 0b00000000;
byte instruction_reg = 0b00000000;
byte output_reg = 0b00000000;
byte memory_reg = 0b00000000;
byte flag_reg = 0b00000000;
byte memory_ram[] = {0b01010001, 0b01001110, 0b01010000, 0b01001111, 0b11100000, 0b00011110, 0b00101111, 0b01001110, 0b11100000, 0b00011111, 0b00101110, 0b01111101, 0b01100011, 0b11110000, 0b00000000, 0b00000000};



void setup() {
  // put your setup code here, to run once:
  matrix.begin();
  Serial.begin(115200); //set up serial library baud rate to 115200
   
  Serial.println("Ben Eater 8-Bit Breadboard Computer Emulator");
  Serial.println("---------");
  Serial.println("Initialise");
  Serial.println("---------");
  Serial.print("A Register Set to: ");
  printByteContents(a_reg); 
  Serial.print("B Register Set to: ");
  printByteContents(b_reg); 
  Serial.print("Instruction Register Set to: ");
  printByteContents(instruction_reg); 
  Serial.print("Output Register Set to: ");
  printByteContents(output_reg); 
  Serial.print("Memory Address Register Set to: ");
  printByteContents(memory_reg); 
  Serial.print("RAM Array Set to: ");
  printByteArrayContents(memory_ram);
  Serial.println("Init Complete");
  Serial.println("---------");
  delay(clock_delay_value * 5);



}

void loop() {
// Main Program Counter
while(counter_enabled == true && prog_counter <=15) {
//for(prog_counter;prog_counter <= 15;) {
  Serial.print("Program Counter = ");
  Serial.println(prog_counter);


  update_reg_display(a_x_axis, a_reg);
  update_reg_display(b_x_axis, b_reg);
  update_reg_display(inst_x_axis, instruction_reg);
  update_reg_display(out_x_axis, output_reg);
  update_reg_display(memreg_x_axis, memory_reg);
  Serial.println("Updated Display");
  delay(clock_delay_value);


  MI("CO");
  II("RO");
  delay(clock_delay_value * 5);
  // Decode Instruction
  byte instruction = instruction_reg & 0b11110000;
  instruction = instruction >>4;
  Serial.print("Instruction Byte: 0x");
  Serial.println(instruction,HEX);  

  if(instruction == 0) {
    Serial.println("Instruction NOP");
    NOP();
  }
  else if(instruction == 1) {
    Serial.println("Instruction LDA");
    LDA();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 2) {
    Serial.println("Instruction ADD");
    ADD();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 3) {
    Serial.println("Instruction SUB");
    SUB();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 4) {
    Serial.println("Instruction STA");
    STA();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 5) {
    Serial.println("Instruction LDI");
    LDI();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 6) {
    Serial.println("Instruction JMP");
    JMP();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 7) {
    Serial.println("Instruction JC");
    JC();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 8) {
    Serial.println("Instruction JZ");
    JZ();
    delay(clock_delay_value * 2);
  }  
  else if(instruction == 14) {
    Serial.println("Instruction OUT");
    OUT();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 15) {
    Serial.println("Instruction HLT");
    HLT();
    delay(clock_delay_value * 2);
  }
  else {
    Serial.println("Command Not Found");
    MI("CO");
    II("RO");
    CE();
    delay(clock_delay_value * 2);
  }  
}
}


// CPU Instructions
void NOP() {
  MI("CO");
  II("RO");
  CE();
  Serial.println("Command Completed: NOP");
}

void LDA() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  AI("RO");
  Serial.println("Command Completed: LDA");
}

void ADD() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  BI("RO");
  FI();
  AI("AD");
  Serial.println("Command Completed: ADD");
}

void SUB() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  BI("RO");
  FI();
  AI("SU");
  Serial.println("Command Completed: SUB");
}

void STA() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  RI("AO");
  Serial.println("Command Completed: STA");
}

void LDI() {
  MI("CO");
  II("RO");
  CE();
  AI("IO");
  Serial.println("Command Completed: LDI");
}

void JMP() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  J("IO");
  Serial.println("Command Completed: JMP");
}

void JC() {
  MI("CO");
  II("RO");
  CE();
  if(flag_reg == 1 || flag_reg == 3) {
    J("IO");
  }
  Serial.println("Command Completed: JC");
}

void JZ() {
  MI("CO");
  II("RO");
  CE();
  if(flag_reg == 2 || flag_reg == 3) {
    J("IO");
  }
  
  Serial.println("Command Completed: JZ");
}

void OUT() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  OI("AO");
  Serial.println("Command Completed: OUT");
}

void HLT() {
  MI("CO");
  II("RO");
  CE();
  Serial.println("----------------");
  Serial.println("------HALT------");
  Serial.println("----------------");

  while(1) {
   // statement block
  }
}

// Control Signals
void MI(String source) {
  if(source == "CO") {
    memory_reg = CO();
    Serial.print("Memory Address Register Set From Program Counter to: 0x");
    Serial.println(memory_reg,HEX);  
  }
  else if(source == "IO") {
    memory_reg = IO();
    Serial.print("Memory Address Register Set From Instruction Register to: 0x");
    Serial.println(memory_reg,HEX);  
  }  
  update_reg_display(memreg_x_axis, memory_reg);
}

void FI() {
    flag_reg_input_enabled = true;
    Serial.println("Enabling Flag Register Input");
  }  



void J(String source) {
  if(source == "IO") {
    prog_counter = IO();
    Serial.print("Jumping to: 0x");
    Serial.println(memory_reg,HEX);  
  }  
  update_reg_display(memreg_x_axis, memory_reg);
}



void OI(String source) {
  if(source == "AO") {
    output_reg = a_reg;
    Serial.print("Output Register Set From A Register to: 0x");
    Serial.println(memory_reg,HEX);  
    Serial.println("----------------");
    Serial.print("Output is: ");
    Serial.println(output_reg); 
    Serial.println("----------------");
  }

  
  update_reg_display(out_x_axis, output_reg);
}


void AI(String source) {
  if(source == "RO") {
    a_reg = RO();
    Serial.print("A Register Set From RAM to: 0x");
    Serial.println(a_reg,HEX);  
  }  
  else if(source == "IO") {
    a_reg = IO();
    Serial.print("A Register Set From Instruction Register to: 0x");
    Serial.println(a_reg,HEX);  
  }  
  else if(source == "AD") {
    a_reg = EO("AD");
    Serial.print("A Register Set From Adder to: 0x");
    Serial.println(a_reg,HEX);  
  }  
  else if(source == "SU") {
    a_reg = EO("SU");
    Serial.print("A Register Set From Subtractor to: 0x");
    Serial.println(a_reg,HEX);  
  }  
  update_reg_display(a_x_axis, a_reg);
}

void BI(String source) {
  if(source == "RO") {
    b_reg = RO();
    Serial.print("B Register Set From RAM to: 0x");
    Serial.println(b_reg,HEX);  
  }  
  update_reg_display(b_x_axis, b_reg);
}

void RI(String source) {
  if(source == "AO") {
    byte mem_address = memory_reg;
    memory_ram[mem_address] = a_reg;
    Serial.print("RAM Set From A Register to: 0x");
    Serial.print(memory_ram[mem_address],HEX);  
    Serial.print(" At Address: 0x");
    Serial.println(mem_address,HEX);  

    Serial.print("RAM Array Set to: ");
    printByteArrayContents(memory_ram);
  }  

}

byte CO() {
  byte counter_output = prog_counter;
  return counter_output;
}

byte IO() {
  byte instruction_output = instruction_reg & 0b00001111;
  //instruction_output = instruction_output <<4;
  Serial.print("IO Output: ");
  Serial.println(instruction_output, HEX);
  //byte instruction_output = instruction_reg;
  return instruction_output;
}

void CE() {
  if (prog_counter == 15) { // Rollover Counter if = 15
    Serial.println("Program Counter Rolled Over ");
    prog_counter = 0;
    counter_enabled = true;
  }
  else { // Increment Counter if < 15    
    prog_counter = prog_counter + 1;
    counter_enabled = true;
    Serial.print("Counter Increased to: 0x");
    Serial.println(prog_counter,HEX);
  }
}

void II(String source) {
  if(source == "RO") {
    instruction_reg = RO();
    Serial.print("Instruction Register Set From RAM to: 0x");
    Serial.println(instruction_reg,HEX);  }  

  update_reg_display(inst_x_axis, instruction_reg);
}

byte EO(String operand) {
  byte result;
  if(operand == "AD") {
    result = a_reg + b_reg;
    uint16_t long_result = a_reg + b_reg;
    long_result = long_result & 0xFF00;
    Serial.print("Long Result: ");
    Serial.println(long_result);  
    if(flag_reg_input_enabled == true) {
      if(result == 0) {
        flag_reg = 0b00000010;
          Serial.print("Setting Flag Register to: 0x");
          Serial.println(flag_reg,HEX);  
      }
      else if(long_result > 255) {
        flag_reg = 0b00000001;
        Serial.print("Setting Flag Register to: 0x");
        Serial.println(flag_reg,HEX);  
      }
      else if(long_result > 255 || result == 0) {
        flag_reg = 0b00000011;
        Serial.print("Setting Flag Register to: 0x");
        Serial.println(flag_reg,HEX);  
      }
    }
    Serial.print("Adder Result: 0x");
    Serial.println(result,HEX);  
  }
  else if(operand == "SU") {
    result = a_reg - b_reg;
    uint16_t long_result = a_reg + b_reg;
    long_result = long_result & 0xFF00;
    Serial.print("Long Result: ");
    Serial.println(long_result);  
    if(flag_reg_input_enabled == true) {
      if(result == 0) {
        flag_reg = 0b00000010;
        Serial.print("Setting Flag Register to: 0x");
        Serial.println(flag_reg,HEX);  
      }
      else if(long_result > 255) {
        flag_reg = 0b00000001;
        Serial.print("Setting Flag Register to: 0x");
        Serial.println(flag_reg,HEX);  
      }
      else if(long_result > 255 || result == 0) {
        flag_reg = 0b00000011;
        Serial.print("Setting Flag Register to: 0x");
        Serial.println(flag_reg,HEX);  
      }
    }
    Serial.print("Subtractor Result: 0x");
    Serial.println(result,HEX);  
  }
  return result;
}

byte RO() {
  byte ram_val = memory_ram[memory_reg];
  Serial.print("Ram Address: 0x");
  Serial.println(memory_reg,HEX);
  Serial.print("Ram Value: 0x");
  Serial.println(ram_val,HEX);
  return ram_val;


}



byte printByteContents(byte data) {
    Serial.println(data, HEX);  
}

byte printByteArrayContents(byte data[]) {

    char buf[80];
    sprintf(buf, "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
             data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  
}

int update_reg_display (int axis, byte val) {  
    
    frame[7][axis] = int(bitRead(val, 0));
    frame[6][axis] = int(bitRead(val, 1));
    frame[5][axis] = int(bitRead(val, 2));
    frame[4][axis] = int(bitRead(val, 3));
    frame[3][axis] = int(bitRead(val, 4));
    frame[2][axis] = int(bitRead(val, 5));
    frame[1][axis] = int(bitRead(val, 6));
    frame[0][axis] = int(bitRead(val, 7));
    matrix.renderBitmap(frame, 8, 12);

}
