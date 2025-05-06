#pragma once
#include "../Classes.h"

template<typename SyncStrategy>
class SyncDecorator : public FileSystemNode {
    FileSystemNode* wrappedNode;
    SyncStrategy syncStrategy;
public:
    explicit SyncDecorator(FileSystemNode* node) : wrappedNode(node) {}
    ~SyncDecorator() override { delete wrappedNode; }

    void accept(Visitor& visitor) override {
        syncStrategy.lock();
        wrappedNode->accept(visitor);
        syncStrategy.unlock();
    }
    FileSystemNode* getWrapped() const { return wrappedNode; }
};