/* Quinta Essentia by Przemysław Pastuszka
 * file:Timer.cs
 * Contains: Class Timer
 * Timer: class to calculate interval between current and last frame; calculating FPS
 * Last modification:16-07-2009
 */
using System;

namespace QuintaEssentia
{
    class Timer
    {
        //variables
        private double[] intervals; //intervals between last n frames
        private DateTime lastFrame; //time when the last frame occured
        private bool gameStopped = false; //for stopping the game
        //first value indicates how many frames will be used to calculate the FPS. It will be arithmetic average
        private int nrOfFrames = 200, counter=0; //second determines current position in 'intervals' array
        private double sum; //sum of all intervals included in 'intervals' array

        /// <summary>
        /// Preparing timer
        /// </summary>
        public Timer()
        {
            intervals = new double[nrOfFrames];
            for (int i = 0; i < nrOfFrames; i++)
                intervals[i] = 0.0166;
            sum = nrOfFrames * 0.0166;
            lastFrame = DateTime.Now;
        }
        /// <summary>
        /// It should be called when the new frame is about to be calculated
        /// </summary>
        /// <returns>Returns time interval betweens last and current frame</returns>
        public double NewFrame()
        {
            TimeSpan temp = DateTime.Now - lastFrame; //time between the last and current frame
            lastFrame = DateTime.Now;

            sum -= intervals[counter]; //deleting the first known interval
            intervals[counter]=temp.TotalSeconds; //and adding a new one
            sum += temp.TotalSeconds;
            counter = (counter + 1) % nrOfFrames;
            
            if (gameStopped)
                return 0;
            return temp.TotalSeconds;
        }
        public void Pause()
        {
            gameStopped = !gameStopped;
        }
        public double GetFPS()
        {
            return nrOfFrames / sum;
        }
    }
}
