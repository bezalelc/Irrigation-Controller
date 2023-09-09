import { useState } from "react";
import { sendPasswordResetEmail } from "firebase/auth";
import { useNavigate } from "react-router-dom";
import { useFirebase } from "../FirebaseContext";

const ForgotPassword = () => {
    const { firebaseAuth } = useFirebase()
    const [resetEmailSent, setResetEmailSent] = useState(false);
    const navigate = useNavigate()

    const resetHandler = async (event) => {
        event.preventDefault();
        const email = event.target.email.value;

        try {
            await sendPasswordResetEmail(firebaseAuth, email);
            setResetEmailSent(true);
        } catch (error) {
            console.error('Error sending reset email:', error);
        }
    };


    return (
        <>
            {resetEmailSent ? (
                <p>Reset email sent. Please check your email inbox.</p>
            ) : (
                <form onSubmit={resetHandler}>
                    <div>Reset password</div>
                    <input type="email" name="email" placeholder="Enter your email" required />
                    <button type="submit">Reset</button>
                </form>)}
            <button onClick={() => navigate('/')}>Return to login page</button>
        </>
    );
}

export default ForgotPassword;