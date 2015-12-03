/*
 * Copyright (C) 2006-2015  Music Technology Group - Universitat Pompeu Fabra
 *
 * This file is part of Essentia
 *
 * Essentia is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 */

#ifndef ESSENTIA_SPSMODELSYNTH_H
#define ESSENTIA_SPSMODELSYNTH_H


#include "algorithm.h"
#include "algorithmfactory.h"

#include <fstream>

namespace essentia {
namespace standard {

class SpsModelSynth : public Algorithm {

 protected:
  Input<std::vector<Real> > _magnitudes;
  Input<std::vector<Real> > _frequencies;
  Input<std::vector<Real> > _phases;
  Input<std::vector<Real> > _stocenv;
  //Output<std::vector<std::complex<Real> > > _outfft;
  Output<std::vector<Real> > _outframe;

  Real _sampleRate;
  int _fftSize;
  int _hopSize;
  int _stocSize;
  int stocSpecSize;
  Algorithm* _sineModelSynth;
  Algorithm* _stochasticModelSynth;
  // for resample function
 // Algorithm* _fft;
  Algorithm* _ifftSine;
  Algorithm* _overlapAdd;


  int _stocSpecSize;

  void initializeFFT(std::vector<std::complex<Real> >&fft, int sizeFFT);
  //void stochasticModelSynth(const std::vector<Real> stocEnv, const int H, const int N,std::vector<std::complex<Real> > &fftStoc);

// debug
std::ofstream _log;

 public:
  SpsModelSynth() {
    declareInput(_magnitudes, "magnitudes", "the magnitudes of the sinusoidal peaks");
    declareInput(_frequencies, "frequencies", "the frequencies of the sinusoidal peaks [Hz]");
    declareInput(_phases, "phases", "the phases of the sinusoidal peaks");
    declareInput(_stocenv, "stocenv", "the stochastic envelope");
    //declareOutput(_outfft, "fft", "the output FFT frame");
    declareOutput(_outframe, "frame", "the output audio frame");

    _sineModelSynth = AlgorithmFactory::create("SineModelSynth");
    _stochasticModelSynth = AlgorithmFactory::create("StochasticModelSynth");

    // for resample
   // _fft = AlgorithmFactory::create("FFT");
    _ifftSine = AlgorithmFactory::create("IFFT");
    _overlapAdd = AlgorithmFactory::create("OverlapAdd");

  }

  ~SpsModelSynth() {

    delete _sineModelSynth;
    delete _stochasticModelSynth;
  //  delete _fft;
    delete _ifftSine;
    delete _overlapAdd;

    _log.close();
  }

  void declareParameters() {
    declareParameter("fftSize", "the size of the output FFT frame (full spectrum size)", "[1,inf)", 2048);
    declareParameter("hopSize", "the hop size between frames", "[1,inf)", 512);
    declareParameter("sampleRate", "the audio sampling rate [Hz]", "(0,inf)", 44100.);
    declareParameter("stocf", "decimation factor used for the stochastic approximation", "(0,1]", 0.2);
  }

  void configure();
  void compute();

//  void resample(const std::vector<Real> in, std::vector<Real> &out, const int sizeOut);



  static const char* name;
  static const char* description;


};

} // namespace standard
} // namespace essentia

#include "streamingalgorithmwrapper.h"

namespace essentia {
namespace streaming {

class SpsModelSynth : public StreamingAlgorithmWrapper {

 protected:
  Sink<std::vector<Real> > _magnitudes;
  Sink<std::vector<Real> > _frequencies;
  Sink<std::vector<Real> > _phases;
  Sink<std::vector<Real> > _stocenv;
  //Source<std::vector<std::complex<Real> > > _outfft;
  Source<std::vector<Real> > _outframe;

 public:
  SpsModelSynth() {
    declareAlgorithm("SpsModelSynth");
    declareInput(_magnitudes, TOKEN, "magnitudes");
    declareInput(_frequencies, TOKEN, "frequencies");
    declareInput(_phases, TOKEN, "phases");
    declareInput(_stocenv, TOKEN, "stocenv");
   // declareOutput(_outfft, TOKEN, "fft");
    declareOutput(_outframe, TOKEN, "frame");
  }
};

} // namespace streaming
} // namespace essentia


#endif // ESSENTIA_SPSMODELSYNTH_H