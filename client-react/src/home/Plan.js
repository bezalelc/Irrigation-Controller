import { useState } from "react";
import EditPlan from "./EditPlan";
import { GiSandsOfTime } from 'react-icons/gi'
import { MdDateRange, MdOutlineEventRepeat } from 'react-icons/md'
import { BiRepeat } from 'react-icons/bi'
import style from './Plan.module.scss'

const Plan = ({ planId, plan, updatePlansSorted }) => {
    const [editPlanPopUp, setEditPlanPopup] = useState(false)

    const getStringRepaet = () => {
        if (!plan.repeat) {
            return '--'
        }
        
        if (plan.repeatMethod === 'daily') {
            return plan.repeat
        } else if (plan.repeatMethod === 'weekly') {
            return plan.repeat.map(day => `${day + 1}${' '}`)
        }

        return '--'
    }

    return (
        <div className={style.container}>
            <div className={style.wrapper}>
                {/* <div className={style.divText}><span className={style.spanFirst}>start time</span><span className={style.spanLast}>{plan.startTime}</span></div> */}
                <div className={`${style.divText} ${style.startTime}`}>{plan.startTime}</div>
                <div className={style.divText} info="Last time the plan was on"><span className={style.spanFirst}>< MdDateRange /></span><span className={style.spanLast}>{plan.lastTime}</span></div>
                <div className={style.divText} info="Duration"><span className={style.spanFirst} ><GiSandsOfTime /></span><span className={style.spanLast}>{plan.duration} min</span></div>
                <div className={style.divText} info="Repeat method"><span className={style.spanFirst}><BiRepeat /></span ><span className={style.spanLast}>{plan.repeatMethod !== 'noRepit' ? plan.repeatMethod : "No repeat"}</span></div>
                <div className={style.divText} info={plan.repeatMethod === 'daily' ? (plan.repeat === 0 ? "One-time plan" : "Days between the bags") : "The days of the week when the program will run"}>
                    <span className={style.spanFirst}><MdOutlineEventRepeat /></span>
                    <span className={style.spanLast}>{getStringRepaet()}</span></div>
                <button className={style.button} onClick={() => updatePlansSorted("remove", planId)}>Remove plan</button>
                <button className={style.button} onClick={() => setEditPlanPopup(true)}>Edit plan</button>
                {editPlanPopUp && <EditPlan plan={plan} planId={planId} updatePlansSorted={updatePlansSorted} setAddPlanPopup={setEditPlanPopup} />}
            </div>
            <div className={style.progressBar}>
                <svg >
                    <defs>
                        <linearGradient id='gradient'>
                            <stop offset="0%" stopColor="rgba(248, 248, 248, 0.1)" />
                            <stop offset="50%" stopColor="rgba(105, 137, 165, 0.1)" />
                            <stop offset="100%" stopColor="rgba(157, 33, 33, 0.1)" />
                        </linearGradient>
                    </defs>
                    <rect width={`${40}%`} height="100%" className={style.rect}
                        stroke="url(#gradient)" fill="url(#gradient)" />
                </svg>
            </div>
        </div>
    );
}

export default Plan;