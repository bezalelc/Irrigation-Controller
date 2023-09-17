import { useEffect, useState } from 'react';
import style from './CircularProgressBar.module.scss'

const radius = 85, width = 200
const dashArray = radius * Math.PI * 2

const CircularProgressBar = ({ plan }) => {
    const [precentage, setPrecentage] = useState(0)
    const dashOffset = dashArray - (dashArray * precentage) / 100;
    const [startHour, startMinute] = plan.startTime.split(':').map(Number);

    useEffect(() => {
        // Function to calculate the percentage based on start time and duration
        const calculatePercentage = () => {
            // Calculate the current time in minutes since midnight
            const now = new Date();
            const currentHour = now.getHours();
            const currentMinute = now.getMinutes();

            // Calculate the total time in minutes from the start time to now
            const totalTimeMinutes = (currentHour * 60 + currentMinute) - (startHour * 60 + startMinute);

            // Calculate the percentage of elapsed time
            const percentage = (totalTimeMinutes / (plan.duration || 1)) * 100; // Ensure plan.duration is at least 1 to avoid division by zero

            // Ensure the percentage is within the 0-100 range
            const clampedPercentage = Math.min(Math.max(percentage, 0), 100);

            return clampedPercentage;
        };

        // Calculate the percentage initially
        const initialPercentage = calculatePercentage();
        setPrecentage(initialPercentage);

        // Update the percentage every minute (you can adjust the interval as needed)
        const interval = setInterval(() => {
            const updatedPercentage = calculatePercentage();
            setPrecentage(updatedPercentage);
        }, 60000); // Update every minute

        // Clean up the interval when the component unmounts
        return () => clearInterval(interval);
    }, [startHour, startMinute, plan.duration]);


    return (
        <div className={style.container}>
            <svg width={width} height={width} viewBox={`0 0 ${width} ${width}`} className={style.svg}  >
                <defs>
                    <linearGradient id='gradient'>
                        <stop offset="0%" stopColor="rgba(1, 65, 149, 0.581)" />
                        <stop offset="50%" stopColor="rgba(0, 55, 30, 0.504)" />
                        <stop offset="100%" stopColor="rgba(35, 138, 0, 0.575)" />
                    </linearGradient>
                </defs>
                <circle cx={width / 2} cy={width / 2} strokeWidth="15px" r={radius} className={style.background} />
                <circle cx={width / 2} cy={width / 2} strokeWidth="15px" r={radius}
                    className={style.progress} style={{ strokeDasharray: dashArray, strokeDashoffset: dashOffset }}
                    transform={`rotate(-90 ${width / 2} ${width / 2})`} stroke="url(#gradient)" fill="url(#gradient)" />
                <text className={style.text} x='50%' y='50%' dy='0.3em' textAnchor='middle' fill="url(#gradient)">{Math.floor(precentage)}%</text>
            </svg>
            {/* <input className={style.input} type="range" min="0" max="100" step="1" value={precentage} onChange={event => setPrecentage(event.target.value)} /> */}
        </div>
    );
}

export default CircularProgressBar;