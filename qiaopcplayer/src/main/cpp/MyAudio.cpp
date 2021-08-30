//
// Created by qiaopc on 2021/8/30.
//

#include "MyAudio.h"

MyAudio::MyAudio(Playstatus *playstatus) {
    this->playstatus = playstatus;
    queue = new MyQueue(playstatus);
}

MyAudio::~MyAudio() {

}
