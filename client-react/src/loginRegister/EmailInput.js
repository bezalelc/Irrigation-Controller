
import { FaEnvelope } from 'react-icons/fa';
import style from './EmailPasswordInput.module.scss'


const EmailInput = ({ email, setEmail }) => {
    return (
        <div className={style.container}>
            <input type="email" className={style.input} placeholder="Email" value={email} onChange={(event) => setEmail(event.target.value)} required />
            <FaEnvelope className={style.icon} />
        </div>
    );
}

export default EmailInput;