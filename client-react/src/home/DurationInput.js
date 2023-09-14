import style from './DurationInput.module.scss'
import InputNumber from '../sharedComponents/InputNumber'

function DurationInput({ plan, setPlan }) {

    const decreament = () => {
        setPlan(plan => ({
            ...plan,
            duration: plan.duration > 0 ? plan.duration - 1 : plan.duration
        }))
    }

    const increament = () => {
        setPlan(plan => ({
            ...plan,
            duration: plan.duration + 1
        }))
    }

    return (
        <div className={style.container}>
            <label className={style.label}>Duration</label>
            <InputNumber val={plan.duration} decreament={decreament} increament={increament} />
        </div >
    );
}

export default DurationInput;
