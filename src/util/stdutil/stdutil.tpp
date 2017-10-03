template <typename PtrIter>
void STDUtil::deleteContainerPtr(PtrIter from, PtrIter to) {
    while (from != to) {
        delete *from;
        from++;
    }
}