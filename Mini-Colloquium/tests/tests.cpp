#include <gtest/gtest.h>
#include "SyncDecorator.h"
#include "CountingVisitor.h"

TEST(FileSystemTest, SingleThreadVisit) {
    auto* folder = new SyncDecorator<NoSyncStrategy>(new Folder());

    auto* wrappedFolder = static_cast<Folder*>(folder->getWrapped()); 
    wrappedFolder->add(new SyncDecorator<NoSyncStrategy>(new TextFile()));
    wrappedFolder->add(new SyncDecorator<NoSyncStrategy>(new ImageFile()));

    CountingVisitor visitor;
    folder->accept(visitor);
    EXPECT_EQ(visitor.getCount(), 3);
    delete folder;
}

TEST(SyncTest, MultiThreadedCounter) {
    const int numThreads = 4;
    const int numVisits = 1000;
    FileSystemNode* node = new SyncDecorator<MutexSyncStrategy>(new Folder());

    std::vector<std::thread> threads;
    std::atomic<int> totalCount{0};

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&totalCount, node]() {
            for (int j = 0; j < numVisits; ++j) {
                CountingVisitor visitor;
                node->accept(visitor);
                totalCount += visitor.getCount();
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(totalCount.load(), numThreads * numVisits); 
    delete node;
}

TEST(EndToEndTest, ComplexStructure) {
    Disk* rawDisk = new Disk();
    Folder* rawFolder = new Folder();
    TextFile* rawTextFile = new TextFile();
    ImageFile* rawImageFile = new ImageFile();

    auto* disk = new SyncDecorator<MutexSyncStrategy>(rawDisk);
    auto* folder = new SyncDecorator<MutexSyncStrategy>(rawFolder);
    auto* textFile = new SyncDecorator<MutexSyncStrategy>(rawTextFile);
    auto* imageFile = new SyncDecorator<MutexSyncStrategy>(rawImageFile);

    static_cast<Folder*>(folder->getWrapped())->add(textFile);
    static_cast<Folder*>(folder->getWrapped())->add(imageFile);
    static_cast<Disk*>(disk->getWrapped())->add(folder);

    CountingVisitor visitor;
    disk->accept(visitor);
    EXPECT_EQ(visitor.getCount(), 4); 

    delete disk; 
}