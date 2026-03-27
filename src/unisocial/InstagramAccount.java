package unisocial;

public class InstagramAccount extends SocialMediaAccount {
    public InstagramAccount(String accountId, String username) {
        super(accountId, username);
    }

    @Override
    public String getPlatformName() {
        return "Instagram";
    }

    @Override
    protected void publishPost(Post post) {
        System.out.printf(
            "[Instagram] @%s posted: \"%s\"%n",
            getUsername(),
            post.getContent()
        );
    }
}
