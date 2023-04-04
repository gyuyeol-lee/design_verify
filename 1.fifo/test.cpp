#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "obj_dir/Vfifo.h"
#include <cstdio>
#include <queue>

// Test scenario
// 1. fifo_full 신호가 뜰 때 까지 write control signal과 write random data를 연속으로 보냄
// 2. empty 신호가 뜰 때 까지 read control signal로 fifo에 저장된 data를 read함
// 3. write data 와 read data를 compare 함 (mismatch 인 경우 error message 출력)


class Fifo {
public:
    Fifo();
    void init();
    void del();
    void write(unsigned char data);
    int read();
    bool is_full();
    bool is_empty();
    Vfifo *m_dut;
};

Fifo::Fifo() {
    m_dut = new Vfifo;
    m_dut->wr = 0;
    m_dut->rd = 0;
    m_dut->data_in = 0;
    m_dut->rstn = 0;
}

void Fifo::init() {
    m_dut->wr = 0;
    m_dut->rd = 0;
    m_dut->data_in = 0;
    m_dut->rstn = 0;
}

void Fifo::write(unsigned char data) {
    m_dut->wr = 1;
    m_dut->data_in = data;
    //printf("wdata: = %x\n",data);
}

int Fifo::read() {
    m_dut->wr = 0;
    m_dut->rd = 1;
    m_dut->eval();
    unsigned char data_out = m_dut->data_out;
    //printf("rdata: = %x\n",data_out);
    return data_out;
}

bool Fifo::is_full() {
    return m_dut->full;
}

bool Fifo::is_empty() {
    return m_dut->empty;
}

int main(int argc, char** argv) {
    vluint64_t sim_time = 0;
    
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;

    std::queue<unsigned char> data_q;
    Fifo fifo;
    fifo.m_dut->trace(m_trace, 5);

    m_trace->open("waveform_class.vcd");

    std::cout<<"Start !!!"<<std::endl;

    fifo.init();

    while (fifo.is_full() == false) {
        fifo.m_dut->clk ^= 1;
        if (sim_time == 5){
            fifo.m_dut->rstn=1;
        }
        if (sim_time > 7){
            if(fifo.m_dut->clk){
                unsigned char rand_data = rand();
                fifo.write(rand_data);
                data_q.push(rand_data);
            }
        }
        fifo.m_dut->eval();
        m_trace->dump(sim_time);
        sim_time++;
    }

    std::cout<<"Done write !!!"<<std::endl;
    
    while (fifo.is_empty() == false) {
        fifo.m_dut->clk ^= 1;
        if(fifo.m_dut->clk){
            unsigned char fifo_rdata = fifo.read();
            if (fifo_rdata != -1) {
                unsigned char cmp_data = data_q.front();
                data_q.pop();
                if(fifo_rdata != cmp_data){
                    printf("ERROR!! Mismatch compare data!!");
                }
                else printf("PASS!! Match compare data rdata: %x wdata: %x\n", fifo_rdata ,cmp_data);
            }
        }
        fifo.m_dut->eval();
        m_trace->dump(sim_time);
        sim_time++;
    }

    std::cout<<"Done read !!!"<<std::endl;
    
    m_trace->dump(sim_time);
    m_trace->close();

    delete m_trace;
    delete fifo.m_dut;
    return 0;
}

//vluint64_t sim_time = 0;
//void Initial();
//int main (int argc, char** argv, char** env){
//    Vfifo *dut = new Vfifo;
//    Verilated::traceEverOn(true);
//    VerilatedVcdC *m_trace = new VerilatedVcdC;
//    dut -> trace(m_trace,5);
//    m_trace->open("waveform.vcd");
//    Initial();
//    while(dut->full==0){
//        dut->clk^=1;
//        if(sim_time>3){
//            dut->rstn=1;
//            if(dut->clk) dut->data_in=rand();
//        }
//        dut->wr=1;
//        dut->eval();
//        m_trace->dump(sim_time);
//        sim_time++;
//    }
//    while(dut->empty==0){
//        dut->clk^=1;
//        dut->wr=0;
//        dut->rd=1;
//        dut->data_out;
//        dut->eval();
//        m_trace->dump(sim_time);
//        sim_time++;
//    }
//    dut->clk^=1;
//    sim_time++;
//    m_trace->dump(sim_time);
//    m_trace->close();
//    delete dut;
//}
//
//void Initial(){
//    Vfifo *dut = new Vfifo;
//    dut->wr=0;
//    dut->rd=0;
//    dut->data_in=0;
//    dut->rstn=0;
//}

