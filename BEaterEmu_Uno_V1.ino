// Declare Variables

int clock_delay_value = 1; // Allows to slow down program by increasing
bool flag_reg_input_enabled = false; //Whether FI is enabled
int prog_counter = 0; // Program counter, only last 4 bits used
bool counter_enabled = true; // Whether to increment counter
bool debug = false; // Whether to print debug command to serial
byte a_reg = 0b00000000;  // A register
byte b_reg = 0b00000000;  // B Register
byte instruction_reg = 0b00000000;  // Instruction Register
byte output_reg = 0b00000000;  // Output Register
byte memory_reg = 0b00000000;  // Memory Address Register, only last 4 bits used
byte flag_reg = 0b00000000;  // Flags Registers, only last 2 bits used

// RAM as Byte array of 16 bytes
byte memory_ram[] = {0b01010001, 0b01001110, 0b01010000, 0b01001111, 0b11100000, 0b00011110, 0b00101111, 0b01001110, 0b11100000, 0b00011111, 0b00101110, 0b01111101, 0b01100011, 0b11110000, 0b00000000, 0b00000000};



void setup() {
  Serial.begin(115200); //set up serial library baud rate to 115200
  // Print Initialisation Screen to Serial
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
// Main Program
while(counter_enabled == true && prog_counter <=15) { 
  if(debug == true) {
    Serial.print("Program Counter = ");
    Serial.println(prog_counter);
    Serial.println("Updated Display");
  }
  delay(clock_delay_value);


  MI("CO");
  II("RO");  
  delay(clock_delay_value * 5);
  // Decode Instruction, ignore address (for now)
  byte instruction = instruction_reg & 0b11110000;  // And to only get the 1st 4 bits
  instruction = instruction >>4; // Shift bits to LSB
  if(debug == true) {
    Serial.print("Instruction Byte: 0x");
    Serial.println(instruction,HEX);
  }  

  if(instruction == 0) { // NOP
    if(debug == true) {
      Serial.println("Instruction NOP");
    }
    NOP();
  }
  else if(instruction == 1) { // LDA
    if(debug == true) {
      Serial.println("Instruction LDA");
    }
    LDA();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 2) { // ADD
    if(debug == true) {
      Serial.println("Instruction ADD");
    }
    ADD();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 3) { // SUB
    if(debug == true) {
      Serial.println("Instruction SUB");
    }
    SUB();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 4) { // STA
    if(debug == true) {
      Serial.println("Instruction STA");
    }
    STA();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 5) { // LDI
    if(debug == true) {
      Serial.println("Instruction LDI");
    }
    LDI();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 6) { // JMP
    if(debug == true) {
      Serial.println("Instruction JMP");
    }
    JMP();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 7) { // JC
    if(debug == true) {
      Serial.println("Instruction JC"); 
    }
    JC();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 8) { // JZ
    if(debug == true) {
      Serial.println("Instruction JZ"); 
    }
    JZ();
    delay(clock_delay_value * 2);
  }  
  else if(instruction == 14) { // OUT
    if(debug == true) {
      Serial.println("Instruction OUT"); 
    }
    OUT();
    delay(clock_delay_value * 2);
  }
  else if(instruction == 15) { // HLT
    if(debug == true) {
      Serial.println("Instruction HLT");
    }
    HLT();
    delay(clock_delay_value * 2);
  }
  else {
    if(debug == true) { // No Command Matched
      Serial.println("Command Not Found");
    }
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
  if(debug == true) {
    Serial.println("Command Completed: NOP");
  }
}

void LDA() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  AI("RO");
  if(debug == true) {
    Serial.println("Command Completed: LDA");
  }
}

void ADD() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  BI("RO");
  FI();
  AI("AD");
  if(debug == true) {
    Serial.println("Command Completed: ADD");
  }
}

void SUB() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  BI("RO");
  FI();
  AI("SU");
  if(debug == true) {
    Serial.println("Command Completed: SUB");
  }
}

void STA() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  RI("AO");
  if(debug == true) {
    Serial.println("Command Completed: STA");
  }
}

void LDI() {
  MI("CO");
  II("RO");
  CE();
  AI("IO");
  if(debug == true) {
    Serial.println("Command Completed: LDI");
  }
}

void JMP() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  J("IO");
  if(debug == true) {
    Serial.println("Command Completed: JMP");
  }
}

void JC() {
  MI("CO");
  II("RO");
  CE();
  if(flag_reg == 1 || flag_reg == 3) {
    J("IO");
  }
  if(debug == true) {
    Serial.println("Command Completed: JC");
  }
}

void JZ() {
  MI("CO");
  II("RO");
  CE();
  if(flag_reg == 2 || flag_reg == 3) {
    J("IO");
  }
  if(debug == true) {
    Serial.println("Command Completed: JZ");
  }
}

void OUT() {
  MI("CO");
  II("RO");
  CE();
  MI("IO");
  OI("AO");
  if(debug == true) {
    Serial.println("Command Completed: OUT");
  }
}

void HLT() {
  MI("CO");
  II("RO");
  CE();
  Serial.println("----------------");
  Serial.println("------HALT------");
  Serial.println("----------------");

  while(1) {
   // Infinite Loop until Arduino is reset
  }
}

// Control Signals
void MI(String source) {
  if(source == "CO") {
    memory_reg = CO();
    if(debug == true) {
      Serial.print("Memory Address Register Set From Program Counter to: 0x");
      Serial.println(memory_reg,HEX);  
    }
  }
  else if(source == "IO") {
    memory_reg = IO();
    if(debug == true) {
      Serial.print("Memory Address Register Set From Instruction Register to: 0x");
      Serial.println(memory_reg,HEX);  
    }
  }  

}

void FI() {
    flag_reg_input_enabled = true;
    if(debug == true) {
      Serial.println("Enabling Flag Register Input");

    }
  }  

void J(String source) {
  if(source == "IO") {
    prog_counter = IO();
    if(debug == true) {
      Serial.print("Jumping to: 0x");
      Serial.println(memory_reg,HEX);  
    }
  }  

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

}


void AI(String source) {
  if(source == "RO") {
    a_reg = RO();
    if(debug == true) {
      Serial.print("A Register Set From RAM to: 0x");
      Serial.println(a_reg,HEX);  
    }
  }  
  else if(source == "IO") {
    a_reg = IO();
    if(debug == true) {
      Serial.print("A Register Set From Instruction Register to: 0x");
      Serial.println(a_reg,HEX);  
    }
  }  
  else if(source == "AD") {
    a_reg = EO("AD");
    if(debug == true) {
      Serial.print("A Register Set From Adder to: 0x");
      Serial.println(a_reg,HEX);  
    }
  }  
  else if(source == "SU") {
    a_reg = EO("SU");
    if(debug == true) {
      Serial.print("A Register Set From Subtractor to: 0x");
      Serial.println(a_reg,HEX);  
    }
  }  
}

void BI(String source) {
  if(source == "RO") {
    b_reg = RO();
    if(debug == true) {
      Serial.print("B Register Set From RAM to: 0x");
      Serial.println(b_reg,HEX);  
    }
  }  
}

void RI(String source) {
  if(source == "AO") {
    byte mem_address = memory_reg;
    memory_ram[mem_address] = a_reg;
    if(debug == true) {
      Serial.print("RAM Set From A Register to: 0x");
      Serial.print(memory_ram[mem_address],HEX);  
      Serial.print(" At Address: 0x");
      Serial.println(mem_address,HEX);  

      Serial.print("RAM Array Set to: ");
      printByteArrayContents(memory_ram);
    }
  }  

}

byte CO() {
  byte counter_output = prog_counter;
  return counter_output;
}

byte IO() {
  byte instruction_output = instruction_reg & 0b00001111;
  if(debug == true) {
    Serial.print("IO Output: ");
    Serial.println(instruction_output, HEX);
  }
  return instruction_output;
}

void CE() {
  if (prog_counter == 15) { // Rollover Counter if = 15
    if(debug == true) {
      Serial.println("Program Counter Rolled Over ");
    }
    prog_counter = 0;
    counter_enabled = true;
  }
  else { // Increment Counter if < 15    
    prog_counter = prog_counter + 1;
    counter_enabled = true;
    if(debug == true) {
      Serial.print("Counter Increased to: 0x");
      Serial.println(prog_counter,HEX);
    }
  }
}

void II(String source) {
  if(source == "RO") {
    instruction_reg = RO();
    if(debug == true) {
      Serial.print("Instruction Register Set From RAM to: 0x");
      Serial.println(instruction_reg,HEX);  
    }  
  }

}

byte EO(String operand) {
  byte result;
  if(operand == "AD") {
    result = a_reg + b_reg;
    uint16_t long_result = a_reg + b_reg;
    long_result = long_result & 0xFF00;
    if(debug == true) {
      Serial.print("Long Result: ");
      Serial.println(long_result);  
    }
    if(flag_reg_input_enabled == true) {
      if(result == 0) {
        flag_reg = 0b00000010;
        if(debug == true) {
          Serial.print("Setting Flag Register to: 0x");
          Serial.println(flag_reg,HEX);  
        }
      }
      else if(long_result > 255) {
        flag_reg = 0b00000001;
        if(debug == true) {
          Serial.print("Setting Flag Register to: 0x");
          Serial.println(flag_reg,HEX); 
        } 
      }
      else if(long_result > 255 || result == 0) {
        flag_reg = 0b00000011;
        if(debug == true) {
          Serial.print("Setting Flag Register to: 0x");
          Serial.println(flag_reg,HEX);  
        }
      }
    }
    if(debug == true) {
      Serial.print("Adder Result: 0x");
      Serial.println(result,HEX);  
    }
  }
  else if(operand == "SU") {
    result = a_reg - b_reg;
    uint16_t long_result = a_reg + b_reg;
    long_result = long_result & 0xFF00;
    if(debug == true) {
      Serial.print("Long Result: ");
      Serial.println(long_result);  
    }
    if(flag_reg_input_enabled == true) {
      if(result == 0) {
        flag_reg = 0b00000010;
        if(debug == true) {
          Serial.print("Setting Flag Register to: 0x");
          Serial.println(flag_reg,HEX);  
        }
      }
      else if(long_result > 255) {
        flag_reg = 0b00000001;
        if(debug == true) {
          Serial.print("Setting Flag Register to: 0x");
          Serial.println(flag_reg,HEX);  
        }
      }
      else if(long_result > 255 || result == 0) {
        flag_reg = 0b00000011;
        if(debug == true) {
          Serial.print("Setting Flag Register to: 0x");
          Serial.println(flag_reg,HEX);  
        }
      }
    }
    if(debug == true) {
      Serial.print("Subtractor Result: 0x");
      Serial.println(result,HEX);  
    }
  }
  return result;
}

byte RO() {
  byte ram_val = memory_ram[memory_reg];
  if(debug == true) {
    Serial.print("Ram Address: 0x");
    Serial.println(memory_reg,HEX);
    Serial.print("Ram Value: 0x");
    Serial.println(ram_val,HEX);
  }
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

