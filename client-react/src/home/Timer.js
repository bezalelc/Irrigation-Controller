import React, { useState, useEffect } from 'react';
import { BsStopwatch } from 'react-icons/bs';
import style from './Timer.module.scss'

function Timer() {
    const [seconds, setSeconds] = useState(0);

    useEffect(() => {
        // Start the timer when the component mounts
        const timerInterval = setInterval(() => {
            setSeconds((prevSeconds) => prevSeconds + 1);
        }, 1000); // Update every 1 second (1000 milliseconds)

        // Clean up the timer when the component unmounts
        return () => {
            clearInterval(timerInterval);
        };
    }, []); // Empty dependency array to run this effect only once when the component mounts

    // Format the seconds into hours, minutes, and seconds
    const formattedTime = new Date(seconds * 1000).toISOString().substring(11, 19);

    return (
        <div className={style.container} info="The time elapsed since the tap was opened">
            <div className={style.iconContainer}>
                <BsStopwatch className={style.icon} />
            </div>
            <div className={style.div}>{formattedTime}</div>
        </div>
    );
}

export default Timer;
