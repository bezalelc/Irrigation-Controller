import React, { useState, useEffect } from 'react';
import { BsStopwatch } from 'react-icons/bs';
import style from './Timer.module.scss';

function Timer({ openTime, isOpen }) {
    const [elapsedTime, setElapsedTime] = useState(0);

    // Parse the openTime string to get the target time in milliseconds
    const [targetHours, targetMinutes, targetSeconds] = openTime.split(':').map(Number);
    const targetTime = targetHours * 3600 + targetMinutes * 60 + targetSeconds;

    useEffect(() => {
        // Start the timer when the component mounts
        const timerInterval = setInterval(() => {
            // Calculate the time elapsed since openTime
            const now = new Date();
            const currentHours = now.getHours();
            const currentMinutes = now.getMinutes();
            const currentSeconds = now.getSeconds();
            const currentTime = currentHours * 3600 + currentMinutes * 60 + currentSeconds;
            const elapsed = currentTime >= targetTime ? currentTime - targetTime : 86400 - targetTime + currentTime;

            setElapsedTime(elapsed);
        }, 1000); // Update every 1 second (1000 milliseconds)

        // Clean up the timer when the component unmounts
        return () => {
            clearInterval(timerInterval);
        };
    }, [targetTime]);

    // Format the elapsed time into hours, minutes, and seconds
    const hours = Math.floor(elapsedTime / 3600);
    const minutes = Math.floor((elapsedTime % 3600) / 60);
    const seconds = elapsedTime % 60;
    const formattedTime = `${String(hours).padStart(2, '0')}:${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}`;

    return (
        <div className={style.container} info="The time elapsed since the tap was opened">
            <div className={style.iconContainer}>
                <BsStopwatch className={style.icon} />
            </div>
            <div className={style.div}>{isOpen ? formattedTime : "Waiting..."}</div>
        </div>
    );
}

export default Timer;
