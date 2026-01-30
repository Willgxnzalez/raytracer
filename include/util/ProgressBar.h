#pragma once
#include <iostream>

class ProgressBar {
private:
    std::ostream &out_;
    int totalSteps_;
    int currentStep_;
    int barWidth_;

public:
    ProgressBar(std::ostream& out, int totalSteps, int barWidth = 40) 
        : out_(out), totalSteps_(totalSteps), currentStep_(0), barWidth_(barWidth) {}

    void update() {
        currentStep_ += 1;
        display();
    }

private:
    void display() {
        double progress = static_cast<double>(currentStep_) / totalSteps_;
        int filledWidth = static_cast<int>(progress * barWidth_);
        
        out_ << "\r[";
        for (int i = 0; i < barWidth_; ++i) {
            if (i <= filledWidth) {
                out_ << "#";
            } else {
                out_ << " ";
            }
        }

        out_ << "] " << std::setw(3) << static_cast<int>(progress * 100) << "%";
        out_ << std::flush;
        
        if (currentStep_ >= totalSteps_) {
            out_ << "\n";
        }
    }
};