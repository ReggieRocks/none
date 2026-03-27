package unisocial;

public final class UniSocialApp {
    private UniSocialApp() {
    }

    public static void main(String[] args) {
        UniSocialHub hub = new UniSocialHub("Campus Connect Hub");

        AccountObserver instagram = new InstagramAccount("insta-01", "campus.life");
        AccountObserver facebook = new FacebookAccount("fb-01", "Campus Community");
        AccountObserver twitter = new TwitterAccount("x-01", "@campus_daily");

        // 1) Add social media accounts to UniSocial
        hub.addSocialAccount(instagram);
        hub.addSocialAccount(facebook);
        hub.addSocialAccount(twitter);

        // 2) Broadcast one post to all connected accounts
        hub.broadcastPost("Welcome back students! Club fair starts at 4 PM in the main quad.");

        // 3) Remove an account from UniSocial
        hub.removeSocialAccount("fb-01");

        // 4) Broadcast again (Facebook no longer receives this post)
        hub.broadcastPost("Reminder: Midterm prep workshop tonight at 7 PM in Library Hall.");
    }
}
