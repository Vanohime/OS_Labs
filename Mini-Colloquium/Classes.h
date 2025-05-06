#include <iostream>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#pragma once

class Visitor;

class FileSystemNode {
public:
    virtual ~FileSystemNode() = default;
    virtual void accept(Visitor& visitor) = 0;
};

class Disk : public FileSystemNode {
    std::vector<FileSystemNode*> children;
public:
    void add(FileSystemNode* node) {
        children.push_back(node);
    }

    void accept(Visitor& visitor) override;

    ~Disk() override {
        for (auto* child : children) {
            delete child;
        }
    }

};

class Folder : public FileSystemNode {
    std::vector<FileSystemNode*> children;
public:
    void add(FileSystemNode* node) {
        children.push_back(node);
    }

    void accept(Visitor& visitor) override;

    ~Folder() override {
        for (auto* child : children) {
            delete child;
        }
    }
};

class File : public FileSystemNode {
public:
    virtual std::string getType() const = 0;
};

class TextFile : public File {
public:
    void accept(Visitor& visitor) override;
    std::string getType() const override { return "Text"; }
};

class ImageFile : public File {
public:
    void accept(Visitor& visitor) override;
    std::string getType() const override { return "Image"; }
};

class Visitor {
public:
    virtual void visit(Disk& disk) = 0;
    virtual void visit(Folder& folder) = 0;
    virtual void visit(TextFile& file) = 0;
    virtual void visit(ImageFile& file) = 0;
};

void Disk::accept(Visitor& visitor) {
    visitor.visit(*this);
    for (auto* child : children) {
        child->accept(visitor);
    }
}

void Folder::accept(Visitor& visitor) {
    visitor.visit(*this);
    for (auto child : children) {
        child->accept(visitor);
    }
}

void TextFile::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void ImageFile::accept(Visitor& visitor) {
    visitor.visit(*this);
}


struct MutexSyncStrategy {
    std::mutex mtx;
    void lock() { mtx.lock(); }
    void unlock() { mtx.unlock(); }
};

struct NoSyncStrategy {
    void lock() {}
    void unlock() {}
};

