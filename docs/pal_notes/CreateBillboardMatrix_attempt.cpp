const NaMATRIX<float, 4, 4>& CreateBillboardMatrix(const NaMATRIX<float, 4, 4>& mat) {
    NaVECTOR<float, 4> cam_pos = D_00383638->unk50;
    cam_pos[3] = 1.0f;

    NaVECTOR<float, 4> local_pos;
    PrRawMat inv_mat;
    {
        PrRawMat tmp_mat;
        {
            int w = 3;
            do { w--; } while (w != -1);
        }
        func_00165FA8(tmp_mat.m, *(const sceVu0FMATRIX*)&mat);
        {
            int w = 3;
            do { w--; } while (w != -1);
        }
        MatCopy((NaMATRIX<float, 4, 4>*)&inv_mat, (NaMATRIX<float, 4, 4>*)&tmp_mat);
    }
    {
        NaVECTOR<float, 4> tmp_vec;
        asm volatile(
            "lqc2     $vf4,   0x0(%1)       \n\t"
            "lqc2     $vf5,  0x10(%1)       \n\t"
            "lqc2     $vf6,  0x20(%1)       \n\t"
            "lqc2     $vf7,  0x30(%1)       \n\t"
            "lqc2     $vf8,   0x0(%2)       \n\t"
            "vmulax.xyzw   ACC,    $vf4,   $vf8  \n\t"
            "vmadday.xyzw  ACC,    $vf5,   $vf8  \n\t"
            "vmaddaz.xyzw  ACC,    $vf6,   $vf8  \n\t"
            "vmaddw.xyzw   $vf9,   $vf7,   $vf8  \n\t"
            "sqc2     $vf9,   0x0(%0)       \n\t"
        : : "r"(&tmp_vec), "r"(&inv_mat), "r"(&cam_pos));
        local_pos = tmp_vec;
    }

    if (local_pos[0] == 0.0f && local_pos[2] == 0.0f) {
        return NaMATRIX<float, 4, 4>::IDENT;
    }

    float angle = atan2f(local_pos[0], local_pos[2]);
    static NaMATRIX<float, 4, 4> return_matrix;
    return_matrix = func_0014ACE8(1, angle);
    return return_matrix;
}
