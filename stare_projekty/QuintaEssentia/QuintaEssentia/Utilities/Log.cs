/* Quinta Essentia by Przemysław Pastuszka
 * file:Log.cs
 * Contains: Class Log, enum Priority
 * Log: everyone knows
 * Priority: each event needs to have defined priority and Priority enum enables it
 * Last modification:24-07-2009
 */
using System;
using System.Collections.Generic;
using System.IO;

namespace QuintaEssentia
{
    enum Priority { low, medium, high };
    class Log
    {
        /// <summary>
        /// It contains information about one event which is going to be saved in the log.
        /// It is seen only inside 'Log' class
        /// </summary>
        class Event
        {
            public String text;
            public Priority priority;
            public DateTime time;
            public Event(String a, Priority b)
            {
                text = a; priority = b;
                time = DateTime.Now;
            }
        }

        //variables
        private List<Event> events;
        //it is not a real singleton, because each module shoud be allowed to have its own log
        //but here we got static member which is the 'global' log
        static public Log mainLog; 

        /// <summary>
        /// Constructor of 'Log' class. It is not a singleton, so the constructor stays public
        /// </summary>
        public Log()
        {
            events = new List<Event>();
        }

        //event handling
        /// <summary>
        /// Add event to the log
        /// </summary>
        /// <param name="text">Message</param>
        /// <param name="priority">Priority</param>
        public void AddEvent(String text, Priority priority)
        {
            Event temp = new Event(text, priority);
            events.Add(temp);
            if(Interface.mainInterface!=null) //if there is a console, just display the event there
                ((Console)Interface.mainInterface.GetModuleByIndex(0)).AddLine("Log: " + text); //the event is automatically displayed on the console
        }
        /// <summary>
        /// Saves all events using the html format
        /// </summary>
        /// <param name="file">Path of the file to be written</param>
        public void Save(String file)
        {
            StreamWriter writer = new StreamWriter(file);
            foreach (Event a in events)
            {
                if (a.priority == Priority.low)
                    writer.WriteLine("{1} - {0}<br/>", a.text,a.time);
                if (a.priority == Priority.medium)
                    writer.WriteLine("<b>{1} - {0}</b><br/>", a.text, a.time);
                if (a.priority == Priority.high)
                    writer.WriteLine("<font color=red><b>{1} - {0}</b></font><br/>", a.text, a.time);
            }
            writer.Close();
        }
        /// <summary>
        /// Deletes all events in the log
        /// </summary>
        public void Clear()
        {
            events.Clear();
        }
    }
}
