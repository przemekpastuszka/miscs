/* Quinta Essentia by Przemysław Pastuszka
 * file:StdVector.cs
 * Contains: Class StdVector
 * StdVector: class that provides the functionality of std::vector in C++
 * Last modification:22-07-2009
 */

namespace QuintaEssentia
{
    class StdVector<T>
    {
        //variables
        private T[] array; //elements
        private int currentElement=-1; //nr of last element in array

        //constructor
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="size">How much memory should be reserved at the beginning</param>
        public StdVector(int size)
        {
            array = new T[size];
        }

        //StdVector functionality
        /// <summary>
        /// Add item to StdVector
        /// </summary>
        /// <param name="item">item</param>
        public void Add(T item)
        {
            currentElement++;
            if (currentElement == array.Length)
            {
                T[] temp = new T[currentElement * 2];
                array.CopyTo(temp, 0);
                array = temp;
            }
            array[currentElement] = item;
        }
        /// <summary>
        /// Returns nr of elements in stdVector
        /// </summary>
        public int Length
        {
            get
            {
                return currentElement+1;
            }
        }
        /// <summary>
        /// Overloaded operator which provides 'arraylike' access
        /// </summary>
        /// <param name="index">Nr of desired item</param>
        /// <returns>Desired item</returns>
        public T this[int index]
        {
            get
            {
                return array[index];
            }
            set
            {
                array[index] = value;
            }
        }
    }
}
