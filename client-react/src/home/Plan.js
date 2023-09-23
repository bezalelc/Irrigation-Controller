import { useEffect, useState } from "react";
import EditPlan from "./EditPlan";
import { GiSandsOfTime } from 'react-icons/gi'
import { MdDateRange, MdOutlineEventRepeat } from 'react-icons/md'
import { BiRepeat } from 'react-icons/bi'
import SubmitButton from "../sharedComponents/SubmitButton";
import style from './Plan.module.scss'


export const repeatMethod = [
    {
        text: "daily",
        info: "Days between the bags",
        repeatText: repeat => repeat
    },
    {
        text: "weekly",
        info: "The days of the week when the program will run",
        repeatText: repeat => repeat ? repeat.map(day => `${day + 1}${' '}`) : '--'
    },
    {
        text: "no repeat",
        info: "One-time plan",
        repeatText: () => '--'
    }
]

const Plan = ({ planId, plan, updatePlansSorted, isActive }) => {
    const [editPlanPopUp, setEditPlanPopup] = useState(false)
    const [precentage, setPrecentage] = useState(0)
    const [startHour, startMinute] = plan.startTime.split(':').map(Number);

    useEffect(() => {
        // Function to calculate the percentage based on start time and duration
        const calculatePercentage = () => {
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

        if (isActive) {
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
        }
    }, [isActive, startHour, startMinute, plan.duration]);

    const getFormattedLastTime = () => {
        const dateParts = plan.lastTime.split('.');
        const year = dateParts[2];
        const convertedDateString = `${dateParts[0]}.${dateParts[1]}.${year.length === 2 ? `20${year}` : year}`;
        return convertedDateString;
    };


    return (
        <div className={style.container}>
            <div className={style.wrapper}>
                {/* <div className={style.divText}><span className={style.spanFirst}>start time</span><span className={style.spanLast}>{plan.startTime}</span></div> */}
                <div className={`${style.divText} ${style.startTime}`}>{plan.startTime}</div>
                <div className={style.divText} info="Last time the plan was on">
                    <span className={style.spanFirst}>< MdDateRange /></span>
                    <span className={style.spanLast}>{getFormattedLastTime(plan.lastTime)}</span>
                </div>
                <div className={style.divText} info="Duration">
                    <span className={style.spanFirst} ><GiSandsOfTime /></span>
                    <span className={style.spanLast}>{plan.duration} min</span>
                </div>
                <div className={style.divText} info="Repeat method">
                    <span className={style.spanFirst}><BiRepeat /></span >
                    <span className={style.spanLast}>{repeatMethod[plan.repeatMethod].text}</span>
                </div>
                <div className={style.divText} info={repeatMethod[plan.repeatMethod].info}>
                    <span className={style.spanFirst}><MdOutlineEventRepeat /></span>
                    <span className={style.spanLast}>{repeatMethod[plan.repeatMethod].repeatText(plan.repeat)}</span>
                </div>
                <SubmitButton text='Remove plan' onClick={() => updatePlansSorted("remove", planId)} className={style.button} />
                <SubmitButton text='Edit plan' onClick={() => setEditPlanPopup(true)} className={style.button} />
                {editPlanPopUp && <EditPlan plan={plan} planId={planId} updatePlansSorted={updatePlansSorted} setAddPlanPopup={setEditPlanPopup} />}
            </div>
            {isActive && <div className={style.progressBar}>
                <svg >
                    <defs>
                        <linearGradient id='gradient'>
                            <stop offset="0%" stopColor="rgba(248, 248, 248, 0.1)" />
                            <stop offset="50%" stopColor="rgba(105, 137, 165, 0.1)" />
                            <stop offset="100%" stopColor="rgba(157, 33, 33, 0.1)" />
                        </linearGradient>
                    </defs>
                    <rect width={`${parseInt(precentage)}%`} height="100%" className={style.rect}
                        stroke="url(#gradient)" fill="url(#gradient)" />
                </svg>
            </div>}
        </div>
    );
}

export default Plan;