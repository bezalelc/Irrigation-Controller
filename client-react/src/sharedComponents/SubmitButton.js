import style from './SubmitButton.module.scss'

const SubmitButton = ({ text, onClick, className }) => {
    return (
        <div className={style.container}>
            <button className={`${style.button} ${className}`} onClick={onClick}>{text}</button>
        </div>
    );
}

export default SubmitButton;