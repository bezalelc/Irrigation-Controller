import { useState } from "react";
import { sendPasswordResetEmail } from "firebase/auth";
import { useNavigate } from "react-router-dom";
import { useFirebase } from "../FirebaseContext";
import EmailInput from "./EmailInput";
import style from './LoginRegister.module.scss'

const ForgotPassword = () => {
    const { firebaseAuth } = useFirebase()
    const [resetEmailSent, setResetEmailSent] = useState(false);
    const [email, setEmail] = useState()
    const navigate = useNavigate()

    const resetHandler = async (event) => {
        event.preventDefault();
        try {
            await sendPasswordResetEmail(firebaseAuth, email);
            setResetEmailSent(true);
        } catch (error) {
            console.error('Error sending reset email:', error);
        }
    };


    return (
        <div className={style.container}>
            {resetEmailSent ? (
                <form className={style.form} onSubmit={event => event.preventDefault()}>
                    <h1 className={style.h1}>Reset password</h1>
                    <p className={style.p}>Reset email sent, Please check your email inbox.</p>
                    <button className={style.returnToLogin} onClick={() => navigate('/')}>Return to login page</button>
                </form>
            ) : (
                <form className={style.form} onSubmit={resetHandler}>
                    <h1 className={style.h1}>Reset password</h1>
                    <EmailInput email={email} setEmail={setEmail} />
                    <div className={style.registerSpace}></div>
                    <button className={style.buttonSubmit} type="submit">Reset</button>
                    <button className={style.returnToLogin} onClick={() => navigate('/')}>Return to login page</button>
                </form>)}
        </div>
    );
}

export default ForgotPassword;