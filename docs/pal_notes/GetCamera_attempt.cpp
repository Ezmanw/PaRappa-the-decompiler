// Draft attempt for SpcFileHeader::GetCamera in src/prlib/camera.cpp
// Reached 96/140 instruction match (diffscore 50); remaining difference is
// stack frame size (-176 vs -240) due to ee-gcc 2.95.3 reusing stack slots
// across sequential NaVECTOR/NaMATRIX temporaries whereas 2.9-ee gives each its own slot.

PrPERSPECTIVE_CAMERA* SpcFileHeader::GetCamera(float time) const {
    static PrPERSPECTIVE_CAMERA cam;
    const SpcFileHeaderOverlay *self = reinterpret_cast<const SpcFileHeaderOverlay*>(this);

    if (self->unk88 != NULL) {
        cam.position = *self->unk88->GetValue(time);
    } else {
        cam.position = self->unk40;
    }

    if (self->unk8C != NULL) {
        cam.interest = *self->unk8C->GetValue(time);
    } else {
        cam.interest = self->unk50;
    }

    float roll;
    if (self->unk90 != NULL) {
        roll = *self->unk90->GetValue(time);
    } else {
        roll = self->unk60;
    }

    if (self->unk94 != NULL) {
        cam.field_of_view = *self->unk94->GetValue(time);
    } else {
        cam.field_of_view = self->unk64;
    }

    cam.near_clip = self->unk6C;
    cam.far_clip = self->unk70;
    cam.aspect = self->unk68;

    NaVECTOR<float, 4> dir = cam.interest - cam.position;
    cam.up = dir.Cross(NaMATRIX<float, 4, 4>::IDENT[1]).Cross(dir).Normalize();

    if (roll != 0.0f) {
        cam.up.ApplyMatrix(&NaMATRIX<float, 4, 4>::MakeAxisRot(dir, roll));
    }

    return &cam;
}
