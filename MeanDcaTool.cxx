#include <iostream>
#include "TMath.h"
#include "TF1.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StThreeVectorF.hh"

#include "MeanDcaTool.h"
#include "MeanDcaParams.h"

MeanDcaTool::MeanDcaTool() {
    funcUpperZ = new TF1("z_upper", "[0]+[1]/pow(x, [2])", 1, 600);
    funcUpperXY = new TF1("xy_upper", "[0]+[1]/pow(x, [2])", 1, 600);
    funcLowerZ = new TF1("z_lower", "[0]+[1]/pow(x, [2])", 1, 600);
    funcLowerXY = new TF1("xy_lower", "[0]+[1]/pow(x, [2])", 1, 600);
    SetUpperCurveParZ(1.0, 1.0, 2.0);
    SetUpperCurveParXY(1.0, 1.0, 2.0);
    SetLowerCurveParZ(1.0, -1.0, 2.0);
    SetLowerCurveParXY(1.0, -1.0, 2.0);
    clean();
}

void MeanDcaTool::clean() {
    haveCache = false;
    mDCAz = 0;
    mDCAxy = 0;
    for (int i=0; i<N_MAX_DCA_TRACKS; i++) {
        aDCAz[i] = 0;
        aDCAxy[i] = 0;
    }
}

void MeanDcaTool::SetUpperCurveParZ(double p0, double p1, double p2) {
    funcUpperZ->SetParameters(p0, p1, p2);
}
void MeanDcaTool::SetUpperCurveParXY(double p0, double p1, double p2) {
    funcUpperXY->SetParameters(p0, p1, p2);
}
void MeanDcaTool::SetLowerCurveParZ(double p0, double p1, double p2) {
    funcLowerZ->SetParameters(p0, p1, p2);
}
void MeanDcaTool::SetLowerCurveParXY(double p0, double p1, double p2) {
    funcLowerXY->SetParameters(p0, p1, p2);
}

void MeanDcaTool::ReadParams() {
    std::cout << "[LOG] - From MeanDcaTool: Current data set shoule be: [" << DcaPars::dstName << "]\n";
    std::cout << "[LOG] - From MeanDcaTool: Now reading mean DCA parameters:\n";
    std::cout << "\t [DCAz] upper curve parameters: {" << DcaPars::DcaZUpperPars[0] << ", " << DcaPars::DcaZUpperPars[1] << ", " << DcaPars::DcaZUpperPars[2] << "}\n";
    std::cout << "\t [DCAz] lower curve parameters: {" << DcaPars::DcaZLowerPars[0] << ", " << DcaPars::DcaZLowerPars[1] << ", " << DcaPars::DcaZLowerPars[2] << "}\n";
    std::cout << "\t [sDCAxy] upper curve parameters: {" << DcaPars::DcaXYUpperPars[0] << ", " << DcaPars::DcaXYUpperPars[1] << ", " << DcaPars::DcaXYUpperPars[2] << "}\n";
    std::cout << "\t [sDCAxy] lower curve parameters: {" << DcaPars::DcaXYLowerPars[0] << ", " << DcaPars::DcaXYLowerPars[1] << ", " << DcaPars::DcaXYLowerPars[2] << "}\n";
    SetUpperCurveParZ(
        DcaPars::DcaZUpperPars[0],
        DcaPars::DcaZUpperPars[1],
        DcaPars::DcaZUpperPars[2]
    );
    SetLowerCurveParZ(
        DcaPars::DcaZLowerPars[0],
        DcaPars::DcaZLowerPars[1],
        DcaPars::DcaZLowerPars[2]
    );
    SetUpperCurveParXY(
        DcaPars::DcaXYUpperPars[0],
        DcaPars::DcaXYUpperPars[1],
        DcaPars::DcaXYUpperPars[2]
    );
    SetLowerCurveParXY(
        DcaPars::DcaXYLowerPars[0],
        DcaPars::DcaXYLowerPars[1],
        DcaPars::DcaXYLowerPars[2]
    );
}

bool MeanDcaTool::Make(StPicoDst *pico) {
    int nTracks = pico->numberOfTracks();
    if (nTracks> N_MAX_DCA_TRACKS) {
        std::cout << "[WARNING] - From MeanDcaTool Number of tracks out of range: " << nTracks << " > " << N_MAX_DCA_TRACKS << ".\n";
        return false;
    }

    int nTrk_valid = 0;
    clean();
    StPicoEvent* event = (StPicoEvent*)pico->event();
    if (!event) { return false; }

    TVector3 vertex = event->primaryVertex();
    double vx = vertex.X();
    double vy = vertex.Y();
    double vz = vertex.Z();
    double mField = event->bField();

    for (int i=0; i<nTracks; i++) {
        StPicoTrack* track = (StPicoTrack*)pico->track(i);
        if (!track) { continue; }
        if (!track->isPrimary()) { continue; }
        aDCAz[nTrk_valid] = track->gDCAz(vz);
        aDCAxy[nTrk_valid] = track->helix(mField).geometricSignedDistance(vx, vy);
        nTrk_valid ++;
    }

    if (nTrk_valid == 0) { return false; }

    for (int i=0; i<nTrk_valid; i++) {
        mDCAz += aDCAz[i];
        mDCAxy += aDCAxy[i];
    }
    mDCAz = mDCAz / nTrk_valid;
    mDCAxy = mDCAxy / nTrk_valid;

    haveCache = true;

    return true;
}

bool MeanDcaTool::IsBadMeanDcaZEvent(StPicoDst* pico) {
    bool res = true;
    if (!haveCache) {
        res = Make(pico);
    }
    if (!res) { return true; }
    int refMult = pico->event()->refMult();
    if (funcUpperZ->Eval(refMult) < mDCAz || funcLowerZ->Eval(refMult) > mDCAz) { return true; }
    return false;
}

bool MeanDcaTool::IsBadMeanDcaXYEvent(StPicoDst* pico) {
    bool res = true;
    if (!haveCache) {
        res = Make(pico);
    }
    if (!res) { return true; }
    int refMult = pico->event()->refMult();
    if (funcUpperXY->Eval(refMult) < mDCAxy || funcLowerXY->Eval(refMult) > mDCAxy) { return true; }
    return false;
}
