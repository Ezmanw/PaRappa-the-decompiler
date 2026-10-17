#ifndef PRLIB_ANIMATION_H
#define PRLIB_ANIMATION_H

#define SPA_MAGIC   (0x59238771)
#define SPA_VERSION (2)

#include "common.h"

#include "linkedlist.h"
#include "objectset.h"

#include <eetypes.h>

class SpmNode;
class SpaNodeAnimation;

class SpaFileHeader {
public:
    u_int m_magic;
    u_short m_version;
    u_short m_flags;
    PR_PADDING(unk8, 0xc);
    float unk14;
    PR_PADDING(unk18, 0x20);
    int *unk38;
    PrLinkedList<SpaFileHeader> m_list;
    PrObjectSet<SpaFileHeader> *m_obj_set;
    void *m_user_data;
    u_int unk4C;
    SpaNodeAnimation **unk50;

public:
    void Initialize();

    bool IsNodeVisible(SpmNode *arg0, float arg1) const;

    void ChangePointer();

    template <typename T>
    T* CalculatePointer(T *offset) {
        if (!offset) {
            return NULL;
        }
        return reinterpret_cast<T*>(reinterpret_cast<int>(this) + reinterpret_cast<int>(offset));
    }
};

#endif /* PRLIB_ANIMATION_H */
