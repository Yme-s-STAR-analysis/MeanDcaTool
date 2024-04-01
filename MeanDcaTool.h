#ifndef __MEAN_DCA_TOOL__
#define __MEAN_DCA_TOOL__

/*
    Version: 2.1
    Author: yghuang
    Date: 01.04.2024
*/

#define N_MAX_DCA_TRACKS 5000

class TF1;
class TProfile;
class StPicoDst;

class MeanDcaTool {

    private:
        // note: here xy/XY means sDCAxy
        TF1* funcUpperZ;
        TF1* funcLowerZ;
        TF1* funcUpperXY;
        TF1* funcLowerXY;


        bool haveCache;

        double aDCAz[N_MAX_DCA_TRACKS];
        double aDCAxy[N_MAX_DCA_TRACKS];

    public:

        double mDCAz;
        double mDCAxy;

        MeanDcaTool();
        ~MeanDcaTool(){}

        void clean();

        void SetUpperCurveParZ(double, double, double);
        void SetLowerCurveParZ(double, double, double);
        void SetUpperCurveParXY(double, double, double);
        void SetLowerCurveParXY(double, double, double);
        void ReadParams(); // from header file

        // when you want to fill histogram or profile, use Make() and get mDCAz/xy
        bool Make(StPicoDst* pico); 

        // when you just want to know an event is good or bad
        bool IsBadMeanDcaZEvent(StPicoDst* pico);
        bool IsBadMeanDcaXYEvent(StPicoDst* pico);
};

#endif
