#include "../../includes/test/TestObject.hpp"

using namespace std;

void Test::TestObject::runAllCoreProcessing()
{
    /* 
        * When create signal object, it will automatically gerenate signal base on parameter below
        * You can adjust the signal by adjust the constructor of Signal class 
    */
    unique_ptr<Signal> signal(new Signal(SAMP_FREQ, SIGNAL_DURATION, SIGNAL_AMPLITUDE)); 
    
    /* 
        * t_outputSignal is a 2D vector, each row is a filtered signal.
        * If you don't understand 2D vector, you can just take the output (.txt) in /build for visualization. 
        * I output the signals into separate .txt files in the /build directory, 
          you can use that and do anything with it.
    */

    /* Canonical testing */
    unique_ptr<Filter::CanonicalFilter> canonicalFilter(new Filter::CanonicalFilter(SAMP_FREQ));

    canonicalFilter->setType(LOW_PASS_2, CUTOFF_FREQ, Q_FACTOR);
    canonicalFilter->process(signal->t_inputSignal, signal->t_outputSignal[1]);

    /* Write signal into .txt and plot it out using python in visualize/ */
    signal->writeSignal(signal->t_inputSignal, TestFiles::Input);
    signal->writeSignal(signal->t_outputSignal[1], TestFiles::Output1);

    /* We can hear the sound of the signal by using createSoundSignal() method
       Remember to change the namespace from TestFiles to SoundTestFiles    */
    // signal->createSoundSignal(signal->t_outputSignal[1], SoundTestFiles::Output1);

    /* SVF testing */
    unique_ptr<Filter::StateVariableFilter> svf(new Filter::StateVariableFilter(SAMP_FREQ));
    svf->tuning(CUTOFF_FREQ, Q_FACTOR);
    svf->process(signal->t_inputSignal, signal->t_outputSignal[2], 
                signal->t_outputSignal[3], signal->t_outputSignal[4]); // three outputs: hpass, bpass, lpass
    signal->writeSignal(signal->t_outputSignal[2], TestFiles::Output2);
    signal->writeSignal(signal->t_outputSignal[3], TestFiles::Output3);
    signal->writeSignal(signal->t_outputSignal[4], TestFiles::Output4);

    /* Shelving testing */
    unique_ptr<Filter::ShelvingFilter> shelvingFilter(new Filter::ShelvingFilter(SAMP_FREQ));
    shelvingFilter->setType(LF_BOOST, CUTOFF_FREQ, GAIN_FACTOR);
    shelvingFilter->process(signal->t_inputSignal, signal->t_outputSignal[5]);
    signal->writeSignal(signal->t_outputSignal[5], TestFiles::Output5);
    signal->createSoundSignal(signal->t_outputSignal[5], SoundTestFiles::Output5);

    /* Peak testing */
    unique_ptr<Filter::PeakFilter> peakFilter(new Filter::PeakFilter(SAMP_FREQ));

    /* The set type in peak filter allow you to choose between Canonical implement or direct different equation implement */
    peakFilter->setType(PEAK_BOOST_MODE, CUTOFF_FREQ, GAIN_FACTOR, Q_FACTOR);
    peakFilter->peakProcess(signal->t_inputSignal, signal->t_outputSignal[6]); // please notice that here I used peakProcess(), not process()!
    signal->writeSignal(signal->t_outputSignal[6], TestFiles::Output6);
    signal->createSoundSignal(signal->t_outputSignal[6], SoundTestFiles::Output6);

    /* Biquad Testing*/
    unique_ptr<Filter::BiquadFilter> biquadFilter(new Filter::BiquadFilter(SAMP_FREQ));
    biquadFilter->setParameters(Type::HighShelf, CUTOFF_FREQ, Q_FACTOR, GAIN_FACTOR);
    biquadFilter->process(signal->t_inputSignal, signal->t_outputSignal[7]);
    signal->writeSignal(signal->t_outputSignal[7], TestFiles::Output7);
    signal->createSoundSignal(signal->t_outputSignal[7], SoundTestFiles::Output7);

    /* Parametric Equalizer Testing*/
    unique_ptr<Equalizer::ParametricEqualizer> parametricEqualizer(new Equalizer::ParametricEqualizer(SAMP_FREQ));

   /*
    * The shelf slope = 1.0 by default
    * You can change the shelf slope by calling  parametricEqualizer->setShelfSlope(which, any);
    */
    parametricEqualizer->setShelfSlope(HIGH_SHELF, 2);
    parametricEqualizer->setLowShelf(200, GAIN_EQUALIZER);
    parametricEqualizer->setFirstMidPeak(300, GAIN_EQUALIZER, Q_FACTOR);
    parametricEqualizer->setSecondMidPeak(400, GAIN_EQUALIZER, Q_FACTOR);
    parametricEqualizer->setHighShelf(600, GAIN_EQUALIZER);
    /*
     *   According to coding rule, we need to return output instead of passing output as parameter
     *   So I changed a little in process function of equalizer
     */
    hu_debug(signal->t_inputSignal[11]);
    signal->t_outputSignal[8] = parametricEqualizer->process(signal->t_inputSignal); 
    signal->writeSignal(signal->t_outputSignal[8], TestFiles::Output8);
    signal->createSoundSignal(signal->t_outputSignal[8], SoundTestFiles::Output8);

    /* Limiter Testing */
    unique_ptr<Modules::Limiter> limiter(new Modules::Limiter(-10, 2, 100, 10));
    limiter->process(signal->t_inputSignal);
    signal->writeSignal(signal->t_inputSignal, TestFiles::Output9);

    /* Warning, using NOT scale might be damage your speaker due to un-covert Q-format - check output file before create sound */
    signal->createSoundNotScaleSignal(signal->t_inputSignal, SoundTestFiles::Output9);
}