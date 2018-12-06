// instruction.h
#ifndef INSTRUCTION_H
#define INSTRUCTION_H 

#include <string>
#include <vector>
#include <map>

// global clock cycle
int clock_cycle;
// To convert stage numbers to status symbols
std::vector<std::string> num_to_status = {".", "IF", "ID", "EX", "MEM", "WB", ".", "*", "*", "*", "."};

class Instruction {
public:
    Instruction(const std::string& f, const std::string& s): 
        function(f), printable(s), stage_at_cycle(17,0), is_active(false), is_done(false), stall_count(0) {};
    virtual ~Instruction();

    void increment_stage();
    void add_stall(int num_stalls);
    virtual void write_back(std::map<std::string, int>& registers) const = 0;
    bool data_hazard_with(const Instruction& other) const;
    Instruction* make_nop() const;

    void print() const;
    bool is_branch() const {return false;}
    int get_stage() const {return stage_at_cycle[clock_cycle];}

protected:
    std::string function;
    std::string destination;
    std::string arg1;
    std::string arg2;
    std::vector<int> stage_at_cycle;

private:
    std::string printable;
    bool is_active;     // if the insruction is currently being incremented
    bool is_done;       // if the instruction has been completed
    int stall_count;
};


class Branch : public Instruction {
public:
    Branch(const std::string& f, const std::string& s);
    virtual ~Branch();

    void write_back(std::map<std::string, int>& registers) const {return;};
    virtual bool branch_taken(std::map<std::string, int>& registers) const = 0;

    bool is_branch() const {return true;}
};


class NonBranch : public Instruction {
public:
    NonBranch(const std::string& f, const std::string& s);
    virtual ~NonBranch();

    virtual void write_back(std::map<std::string, int>& registers) const = 0;    
};


class nop : public Instruction {
public:
    nop();
    void write_back(std::map<std::string, int>& registers) const {return;};
};


// Small classes for each instruction type
// Just handle execution of what the instruction does

// add, addi, and, andi, or, ori, slt, slti, beq, bne

class add : public NonBranch {
public:
    add(const std::string& f, const std::string& s) : NonBranch(f,s) {};
    void write_back(std::map<std::string, int>& registers) const;
};

class addi : public NonBranch {
public:
    addi(const std::string& f, const std::string& s) : NonBranch(f,s) {};
    void write_back(std::map<std::string, int>& registers) const;
};

class or_ : public NonBranch {      // g++ doesn't like classes named "or"
public:
    or_(const std::string& f, const std::string& s) : NonBranch(f,s) {};
    void write_back(std::map<std::string, int>& registers) const;
};

class ori : public NonBranch {
public:
    ori(const std::string& f, const std::string& s) : NonBranch(f,s) {};
    void write_back(std::map<std::string, int>& registers) const;
};

class slt : public NonBranch {
public:
    slt(const std::string& f, const std::string& s) : NonBranch(f,s) {};
    void write_back(std::map<std::string, int>& registers) const;
};

class slti : public NonBranch {
public:
    slti(const std::string& f, const std::string& s) : NonBranch(f,s) {};
    void write_back(std::map<std::string, int>& registers) const;
};

class beq : public Branch {
public:
    beq(const std::string& f, const std::string& s) : Branch(f,s) {};
    bool branch_taken(std::map<std::string, int>& registers) const;    
};

class bne : public Branch {
public:
    bne(const std::string& f, const std::string& s) : Branch(f,s) {};
    bool branch_taken(std::map<std::string, int>& registers) const;
};

#endif
