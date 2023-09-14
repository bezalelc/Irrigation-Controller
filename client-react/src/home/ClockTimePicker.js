import style from './ClockTimePicker.module.scss'

function ClockTimePicker({ plan, setPlan }) {
  return (
    <div className={style.container}>
      <label htmlFor="" className={style.label}>Start time</label>
      <input className={style.input}
        type="time"
        value={plan.startTime}
        onChange={event => setPlan(plan => ({
          ...plan,
          startTime: event.target.value
        }))}
        step="900" // Set the step to 15 minutes (900 seconds)
      />
    </div>
  );
}

export default ClockTimePicker;
