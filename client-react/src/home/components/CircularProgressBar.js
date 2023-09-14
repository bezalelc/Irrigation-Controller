import { useState } from 'react';
import style from './CircularProgressBar.module.scss'

const radius = 85, width = 200
const dashArray = radius * Math.PI * 2

const CircularProgressBar = () => {
    const [precentage, setPrecentage] = useState(35)
    const dashOffset = dashArray - (dashArray * precentage) / 100;

    return (
        <div className={style.container}>
            <svg width={width} height={width} viewBox={`0 0 ${width} ${width}`} className={style.svg}  >
                <defs>
                    <linearGradient id='gradient'>
                        <stop offset="0%" stopColor="red" />
                        <stop offset="50%" stopColor="green" />
                        <stop offset="100%" stopColor="blue" />
                    </linearGradient>
                </defs>
                <circle cx={width / 2} cy={width / 2} strokeWidth="15px" r={radius} className={style.background} />
                <circle cx={width / 2} cy={width / 2} strokeWidth="15px" r={radius}
                    className={style.progress} style={{ strokeDasharray: dashArray, strokeDashoffset: dashOffset }}
                    transform={`rotate(-90 ${width / 2} ${width / 2})`} stroke="url(#gradient)" fill="url(#gradient)" />
                <text className={style.text} x='50%' y='50%' dy='0.3em' textAnchor='middle' fill="url(#gradient)">{precentage}%</text>
            </svg>
            {/* <input className={style.input} type="range" min="0" max="100" step="1" value={precentage} onChange={event => setPrecentage(event.target.value)} /> */}
        </div>
    );
}

export default CircularProgressBar;